#include "RasterRenderer.h"

#include <CanvasTriangle.h>

#include "../helper/Draw.h"
#include "../helper/Interpolation.h"

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
            const auto mappedVertex = projectVertexOntoCanvasPoint(window, vertex.position);

            window.setPixelColour(static_cast<int>(mappedVertex.x), static_cast<int>(mappedVertex.y), 0xFFFFFFFF);
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
            mappedVertices.push_back(projectVertexOntoCanvasPoint(window, vertex.position));
        }

        Draw::drawStrokedTriangle(window, CanvasTriangle(mappedVertices[0], mappedVertices[1], mappedVertices[2], Colour(255, 255, 255)));
    }
}

void RasterRenderer::rasterRender(DrawingWindow& window) const
{
    auto** depthBuffer = new float*[window.height];

    for (size_t y = 0; y < window.height; y++)
    {
        depthBuffer[y] = new float[window.width];
        for (size_t x = 0; x < window.width; x++)
        {
            depthBuffer[y][x] = 0.0f;
        }
    }

    for (const auto& triangle : model.triangles)
    {
        auto mappedVertices = std::vector<CanvasPoint>();

        for (const auto vertex : triangle.vertices)
        {
            mappedVertices.push_back(projectVertexOntoCanvasPoint(window, vertex.position));
        }

        drawDepthAwareFilledTriangle(window, CanvasTriangle(mappedVertices[0], mappedVertices[1], mappedVertices[2],
                                                            model.materials.getMaterial(triangle.material).getColour()),
                                     depthBuffer);
    }

    for (size_t y = 0; y < window.height; y++)
    {
        delete[] depthBuffer[y];
    }
    delete[] depthBuffer;
}

CanvasPoint RasterRenderer::projectVertexOntoCanvasPoint(const DrawingWindow& window, const glm::vec3 vertexPosition) const
{
    // Map model space to camera space
    glm::vec3 cameraVertexPosition = vertexPosition - camera.position;

    cameraVertexPosition = cameraVertexPosition * camera.rotation;
    float u = 0;
    float v = 0;

    float depth = 0;

    if (cameraVertexPosition.z != 0)
    {
        u = camera.imagePlaneScaling * camera.focalLength * (-cameraVertexPosition.x / cameraVertexPosition.z)
            + (static_cast<float>(window.width) / 2);

        v = camera.imagePlaneScaling * camera.focalLength * (cameraVertexPosition.y / cameraVertexPosition.z)
            + (static_cast<float>(window.height) / 2);

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

    for (int y = static_cast<int>(glm::floor(vertices[0].y)); y <= static_cast<int>(glm::floor(vertices[2].y)); y++)
    {
        const float rowStartProportion = Interpolation::proportion(vertices[startVertex].y, vertices[startVertex + 1].y,
                                                                   static_cast<float>(y), 0);
        float rowStartX = Interpolation::interpolate(vertices[startVertex].x, vertices[startVertex + 1].x,
                                                                rowStartProportion);
        float rowStartZ = Interpolation::interpolate(vertices[startVertex].depth,
                                                                vertices[startVertex + 1].depth, rowStartProportion);

        const float rowEndProportion = Interpolation::proportion(vertices[0].y, vertices[2].y, static_cast<float>(y), 1);
        float rowEndX = Interpolation::interpolate(vertices[0].x, vertices[2].x, rowEndProportion);
        float rowEndZ = Interpolation::interpolate(vertices[0].depth, vertices[2].depth, rowEndProportion);

        if (rowStartX > rowEndX)
        {
            std::swap(rowStartX, rowEndX);
            std::swap(rowStartZ, rowEndZ);
        }

        for (int x = static_cast<int>(glm::ceil(rowStartX)); x < static_cast<int>(glm::ceil(rowEndX)); x++)
        {
            const float proportion = Interpolation::proportion(rowStartX, rowEndX, static_cast<float>(x), 0);
            const float zInv = Interpolation::interpolate(rowStartZ, rowEndZ, proportion);

            if (zInv > 0 && zInv < 0.7 &&
                x >= 0 && x < static_cast<int>(window.width) &&
                y >= 0 && y < static_cast<int>(window.height) &&
                zInv > depthBuffer[y][x])
            {
                depthBuffer[y][x] = zInv;
                window.setPixelColour(x, y, colourValue);
            }
        }

        if (y == static_cast<int>(vertices[1].y)) startVertex++;
    }
}
