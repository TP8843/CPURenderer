//
// Created by Thomas Parr on 09/10/2024.
//

#include "RasterRenderer.h"

#include <CanvasTriangle.h>

#include "Draw.h"

RasterRenderer::RasterRenderer(Model& model,
                               const glm::vec3 cameraPosition,
                               const float focalLength,
                               const float imagePlaneScaling)
    : cameraPosition(cameraPosition),
      focalLength(focalLength),
      imagePlaneScaling(imagePlaneScaling),
      model(model)
{
}

void RasterRenderer::pointCloudRender(DrawingWindow &window) const
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

void RasterRenderer::wireframeRender(DrawingWindow &window) const
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

void RasterRenderer::rasterRender(DrawingWindow &window) const
{
    for (const auto& triangle : model.triangles)
    {
        auto mappedVertices = std::vector<CanvasPoint>();

        for (const auto vertex : triangle.vertices)
        {
            mappedVertices.push_back(projectVertexOntoCanvasPoint(window, vertex));
        }

        Draw::drawFilledTriangle(window, CanvasTriangle(mappedVertices[0], mappedVertices[1], mappedVertices[2]),
                                 triangle.colour);
    }
}

CanvasPoint RasterRenderer::projectVertexOntoCanvasPoint(DrawingWindow &window, const glm::vec3 vertexPosition) const
{
    // Map model space to camera space
    const glm::vec3 cameraVertexPosition = vertexPosition - cameraPosition;

    float u = imagePlaneScaling * focalLength * (-cameraVertexPosition.x / cameraVertexPosition.z) + (window.width / 2);
    float v = imagePlaneScaling * focalLength * (cameraVertexPosition.y / cameraVertexPosition.z) + (window.height / 2);

    return {u, v};
}
