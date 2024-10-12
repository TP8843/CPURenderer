//
// Created by Thomas Parr on 09/10/2024.
//

#include "RasterRenderer.h"

#include <CanvasTriangle.h>

#include "Draw.h"
#include "Interpolation.h"

RasterRenderer::RasterRenderer(Model& model,
                               const glm::vec3 cameraPosition,
                               const glm::mat3 cameraRotation,
                               const float focalLength,
                               const float imagePlaneScaling)
    : cameraPosition(cameraPosition),
      cameraRotation(cameraRotation),
      focalLength(focalLength),
      imagePlaneScaling(imagePlaneScaling),
      model(model)
{
}

void RasterRenderer::pointCloudRender(DrawingWindow& window) const
{
    for (const auto& triangle : model.triangles)
    {
        for (const auto vertex : triangle.vertices)
        {
            const auto mappedVertex = projectVertexOntoCanvasPoint(window, vertex);

            window.setPixelColour(mappedVertex.x, mappedVertex.y, 0xFFFFFFFF);
        }
    }
}

void RasterRenderer::wireframeRender(DrawingWindow& window) const
{
    for (const auto& triangle : model.triangles)
    {
        auto mappedVertices = std::vector<CanvasPoint>();

        for (const auto vertex : triangle.vertices)
        {
            mappedVertices.push_back(projectVertexOntoCanvasPoint(window, vertex));
        }

        Draw::drawStrokedTriangle(window, CanvasTriangle(mappedVertices[0], mappedVertices[1], mappedVertices[2]),
                                  Colour(255, 255, 255));
    }
}

void RasterRenderer::rasterRender(DrawingWindow& window) const
{
    auto** depthBuffer = new float*[window.height];

    for (int y = 0; y < window.height; y++)
    {
        depthBuffer[y] = new float[window.width];
        for (int x = 0; x < window.width; x++)
        {
            depthBuffer[y][x] = 0.0f;
        }
    }

    for (const auto& triangle : model.triangles)
    {
        auto mappedVertices = std::vector<CanvasPoint>();

        for (const auto vertex : triangle.vertices)
        {
            mappedVertices.push_back(projectVertexOntoCanvasPoint(window, vertex));
        }

        drawDepthAwareFilledTriangle(window, CanvasTriangle(mappedVertices[0], mappedVertices[1], mappedVertices[2]),
                                     triangle.colour, depthBuffer);
    }

    for (int y = 0; y < window.height; y++)
    {
        delete[] depthBuffer[y];
    }
    delete[] depthBuffer;
}

CanvasPoint RasterRenderer::projectVertexOntoCanvasPoint(DrawingWindow& window, const glm::vec3 vertexPosition) const
{
    // Map model space to camera space
    glm::vec3 cameraVertexPosition = vertexPosition - cameraPosition;

    cameraVertexPosition =cameraVertexPosition * cameraRotation;

    float u = imagePlaneScaling * focalLength * (-cameraVertexPosition.x / cameraVertexPosition.z) + (window.width / 2);
    float v = imagePlaneScaling * focalLength * (cameraVertexPosition.y / cameraVertexPosition.z) + (window.height / 2);

    float depth = 0;

    if (cameraVertexPosition.z != 0)
    {
        // Negative due to positive z out the screen
        depth = -1.0f / cameraVertexPosition.z;
    }


    return {u, v, depth};
}

void RasterRenderer::drawDepthAwareFilledTriangle(DrawingWindow& window, const CanvasTriangle& triangle,
                                                  const Colour& colour, float **depthBuffer)
{
    const auto colourValue = (255 << 24) + (colour.red << 16) + (colour.green << 8) + colour.blue;
    auto vertices = std::array<CanvasPoint, 3>(triangle.vertices);

    // Sort triangle based on y value
    if (vertices[0].y > vertices[1].y) std::swap(vertices[0], vertices[1]);
    if (vertices[1].y > vertices[2].y) std::swap(vertices[1], vertices[2]);
    if (vertices[0].y > vertices[1].y) std::swap(vertices[0], vertices[1]);

    int startVertex = 0;

    for (size_t y = glm::floor(vertices[0].y); y <= static_cast<size_t>(glm::floor(vertices[2].y)); y++)
    {
        const float rowStartProportion = Interpolation::proportion(vertices[startVertex].y, vertices[startVertex + 1].y,
                                                                   y, 0);
        float rowStartX = Interpolation::interpolateSingleFloat(vertices[startVertex].x, vertices[startVertex + 1].x,
                                                                rowStartProportion);
        float rowStartZ = Interpolation::interpolateSingleFloat(vertices[startVertex].depth,
                                                                vertices[startVertex + 1].depth, rowStartProportion);

        const float rowEndProportion = Interpolation::proportion(vertices[0].y, vertices[2].y, y, 1);
        float rowEndX = Interpolation::interpolateSingleFloat(vertices[0].x, vertices[2].x, rowEndProportion);
        float rowEndZ = Interpolation::interpolateSingleFloat(vertices[0].depth, vertices[2].depth, rowEndProportion);

        if (rowStartX > rowEndX)
        {
            std::swap(rowStartX, rowEndX);
            std::swap(rowStartZ, rowEndZ);
        }

        for (size_t x = glm::ceil(rowStartX); x < static_cast<size_t>(glm::ceil(rowEndX)); x++)
        {
            const float proportion = Interpolation::proportion(rowStartX, rowEndX, x, 0);
            const float zInv = Interpolation::interpolateSingleFloat(rowStartZ, rowEndZ, proportion);

            if (zInv < 1 && x >= 0 && x < window.width && y >= 0 && y < window.height &&
                zInv > depthBuffer[static_cast<int>(y)][static_cast<int>(x)])
            {
                depthBuffer[y][x] = zInv;
                window.setPixelColour(x, y, colourValue);
            }
        }

        if (y == static_cast<size_t>(vertices[1].y)) startVertex++;
    }
}
