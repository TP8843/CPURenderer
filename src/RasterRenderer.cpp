#include "RasterRenderer.h"

#include <CanvasTriangle.h>

#include "helper/Draw.h"
#include "helper/Interpolation.h"

RasterRenderer::RasterRenderer(Model& model,
                               Camera& camera)
    : model(model),
      camera(camera)
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

        Draw::drawStrokedTriangle(window, CanvasTriangle(mappedVertices[0], mappedVertices[1], mappedVertices[2], Colour(255, 255, 255)));
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

        drawDepthAwareFilledTriangle(window, CanvasTriangle(mappedVertices[0], mappedVertices[1], mappedVertices[2], triangle.colour),
                                     depthBuffer);
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
    glm::vec3 cameraVertexPosition = vertexPosition - camera.position;

    cameraVertexPosition = cameraVertexPosition * camera.rotation;
    float u = 0;
    float v = 0;

    float depth = 0;

    if (cameraVertexPosition.z != 0)
    {
        u = camera.imagePlaneScaling * camera.focalLength * (-cameraVertexPosition.x / cameraVertexPosition.z) + (window.width / 2);
        v = camera.imagePlaneScaling * camera.focalLength * (cameraVertexPosition.y / cameraVertexPosition.z) + (window.height / 2);

        // Negative due to positive z out the screen
        depth = -1.0f / cameraVertexPosition.z;
    }


    return {u, v, depth};
}

void RasterRenderer::drawDepthAwareFilledTriangle(DrawingWindow& window, const CanvasTriangle& triangle,
                                                  float** depthBuffer)
{
    const auto colourValue = (255 << 24) + (triangle.colour.red << 16) + (triangle.colour.green << 8) + triangle.colour.blue;
    auto vertices = std::array<CanvasPoint, 3>(triangle.vertices);

    // Sort triangle based on y value
    if (vertices[0].y > vertices[1].y) std::swap(vertices[0], vertices[1]);
    if (vertices[1].y > vertices[2].y) std::swap(vertices[1], vertices[2]);
    if (vertices[0].y > vertices[1].y) std::swap(vertices[0], vertices[1]);

    int startVertex = 0;

    for (int y = glm::floor(vertices[0].y); y <= static_cast<int>(glm::floor(vertices[2].y)); y++)
    {
        const float rowStartProportion = Interpolation::proportion(vertices[startVertex].y, vertices[startVertex + 1].y,
                                                                   y, 0);
        float rowStartX = Interpolation::interpolate(vertices[startVertex].x, vertices[startVertex + 1].x,
                                                                rowStartProportion);
        float rowStartZ = Interpolation::interpolate(vertices[startVertex].depth,
                                                                vertices[startVertex + 1].depth, rowStartProportion);

        const float rowEndProportion = Interpolation::proportion(vertices[0].y, vertices[2].y, y, 1);
        float rowEndX = Interpolation::interpolate(vertices[0].x, vertices[2].x, rowEndProportion);
        float rowEndZ = Interpolation::interpolate(vertices[0].depth, vertices[2].depth, rowEndProportion);

        if (rowStartX > rowEndX)
        {
            std::swap(rowStartX, rowEndX);
            std::swap(rowStartZ, rowEndZ);
        }

        for (int x = glm::ceil(rowStartX); x < static_cast<int>(glm::ceil(rowEndX)); x++)
        {
            const float proportion = Interpolation::proportion(rowStartX, rowEndX, x, 0);
            const float zInv = Interpolation::interpolate(rowStartZ, rowEndZ, proportion);

            if (zInv > 0 && zInv < 0.7 && x >= 0 && x < window.width && y >= 0 && y < window.height &&
                zInv > depthBuffer[y][x])
            {
                depthBuffer[y][x] = zInv;
                window.setPixelColour(x, y, colourValue);
            }
        }

        if (y == static_cast<int>(vertices[1].y)) startVertex++;
    }
}
