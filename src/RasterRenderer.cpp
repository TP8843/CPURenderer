//
// Created by Thomas Parr on 09/10/2024.
//

#include "RasterRenderer.h"

#include <CanvasTriangle.h>

#include "Draw.h"

RasterRenderer::RasterRenderer(DrawingWindow& window,
                               Model& model,
                               const glm::vec3 cameraPosition,
                               const float focalLength,
                               const float imagePlaneScaling)
    : cameraPosition(cameraPosition),
      focalLength(focalLength),
      imagePlaneScaling(imagePlaneScaling),
      model(model),
      window(window)
{
}

void RasterRenderer::pointCloudRender() const
{
    for (const auto& triangle : model.triangles)
    {
        for (const auto vertex : triangle.vertices)
        {
            const auto mappedVertex = projectVertexOntoCanvasPoint(vertex);

            window.setPixelColour(mappedVertex.x, mappedVertex.y, 0xFFFFFFFF);
        }
    }
}

void RasterRenderer::wireframeRender() const
{
    for (const auto& triangle : model.triangles)
    {
        auto mappedVertices = std::vector<CanvasPoint>();

        for (const auto vertex : triangle.vertices)
        {
            mappedVertices.push_back(projectVertexOntoCanvasPoint(vertex));
        }

        Draw::drawStrokedTriangle(window, CanvasTriangle(mappedVertices[0], mappedVertices[1], mappedVertices[2]),
                                  Colour(255, 255, 255));
    }
}

void RasterRenderer::rasterRender() const
{
    for (const auto& triangle : model.triangles)
    {
        auto mappedVertices = std::vector<CanvasPoint>();

        for (const auto vertex : triangle.vertices)
        {
            mappedVertices.push_back(projectVertexOntoCanvasPoint(vertex));
        }

        Draw::drawFilledTriangle(window, CanvasTriangle(mappedVertices[0], mappedVertices[1], mappedVertices[2]),
                                 triangle.colour);
    }
}

CanvasPoint RasterRenderer::projectVertexOntoCanvasPoint(const glm::vec3 vertexPosition) const
{
    // Map model space to camera space
    const glm::vec3 cameraVertexPosition = vertexPosition - cameraPosition;

    float u = imagePlaneScaling * focalLength * (-cameraVertexPosition.x / cameraVertexPosition.z) + (window.width / 2);
    float v = imagePlaneScaling * focalLength * (cameraVertexPosition.y / cameraVertexPosition.z) + (window.height / 2);

    return {u, v};
}
