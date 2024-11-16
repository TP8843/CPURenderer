#include "RasterRenderer.h"

#include "../helper/Draw.h"
#include "../shaders/FragmentShaders.h"
#include "../shaders/FragmentData.h"

RasterRenderer::RasterRenderer(Model& model, Transformation& camera, Transformation& light) :
    model(model), camera(camera), light(light)
{
}

void RasterRenderer::pointCloudRender(DrawingWindow& window) const
{
    for (const auto& triangle : model.triangles)
    {
        for (const auto vertex : triangle.vertices)
        {
            const auto mappedVertex = projectVertexOntoCanvasPoint(vertex, window.width, window.height, camera.scale);

            window.setPixelColour(static_cast<int>(mappedVertex.x), static_cast<int>(mappedVertex.y), 0xFFFFFFFF);
        }
    }
}

void RasterRenderer::wireframeRender(DrawingWindow& window) const
{
    for (const auto& triangle : model.getRasterPreparedTriangles(camera))
    {
        auto mappedVertices = std::vector<CanvasPoint>();

        for (const auto vertex : triangle.vertices)
        {
            mappedVertices.push_back(projectVertexOntoCanvasPoint(vertex, window.width, window.height, camera.scale));
        }

        Draw::drawStrokedTriangle(window, CanvasTriangle(mappedVertices[0], mappedVertices[1], mappedVertices[2],
                                                         Colour(255, 255, 255)));
    }
}

void RasterRenderer::rasterRender(DrawingWindow& window) const
{
    std::vector<ModelTriangle> clippedTriangles = model.getRasterPreparedTriangles(camera);

    // Pre pass to pre calculate depth values to avoid multiple shader calls per pixel.
    auto** depthBuffer = generateDepthBuffer(clippedTriangles, window.width, window.height);

    for (const auto& triangle : clippedTriangles)
    {
        Material& material = model.materials.getMaterial(triangle.material);

        CanvasPoint v0 = projectVertexOntoCanvasPoint(triangle.vertices[0], window.width, window.height, camera.scale);
        CanvasPoint v1 = projectVertexOntoCanvasPoint(triangle.vertices[1], window.width, window.height, camera.scale);
        CanvasPoint v2 = projectVertexOntoCanvasPoint(triangle.vertices[2], window.width, window.height, camera.scale);

        // Placeholder colour to allow original raster to work
        const auto canvasTriangle = CanvasTriangle(v0, v1, v2, Colour());
        const auto uniform = FragmentData::DataUniform(window, depthBuffer, material, camera, light, triangle.normal);

        if (!material.hasTexture())
        {
            FragmentData::FilledData d0 = {glm::vec3(1.0f, 0.0f, 0.0f),
                v0.depth,
                triangle.vertices.at(0) * v0.depth,
                triangle.vertexNormals.at(0) * v0.depth};

            FragmentData::FilledData d1 = {glm::vec3(0.0f, 1.0f, 0.0f),
                v1.depth,
                triangle.vertices.at(1) * v1.depth,
            triangle.vertexNormals.at(1) * v1.depth};

            FragmentData::FilledData d2 = {glm::vec3(0.0f, 0.0f, 1.0f),
                v2.depth,
                triangle.vertices.at(2) * v2.depth,
            triangle.vertexNormals.at(2) * v2.depth};

            std::array<FragmentData::FilledData, 3> data = {d0, d1, d2};

            drawTriangle<FragmentData::DataUniform, FragmentData::FilledData>(
                canvasTriangle, uniform, data, FragmentShaders::filled);
        }
        else
        {
            FragmentData::TextureData d1 = {
                triangle.texturePoints[0] * v0.depth, glm::vec3(1.0f, 0.0f, 0.0f), v0.depth,
                triangle.vertices[0] * v0.depth, triangle.vertexNormals.at(0) * v0.depth
            };
            FragmentData::TextureData d2 = {
                triangle.texturePoints[1] * v1.depth, glm::vec3(0.0f, 1.0f, 0.0f), v1.depth,
                triangle.vertices[1] * v1.depth, triangle.vertexNormals.at(1) * v1.depth
            };
            FragmentData::TextureData d3 = {
                triangle.texturePoints[2] * v2.depth, glm::vec3(0.0f, 0.0f, 1.0f), v2.depth,
                triangle.vertices[2] * v2.depth, triangle.vertexNormals.at(2) * v2.depth
            };

            std::array<FragmentData::TextureData, 3> data = {d1, d2, d3};

            drawTriangle<FragmentData::DataUniform, FragmentData::TextureData>(
                canvasTriangle, uniform, data, FragmentShaders::materialPhong);
        }
    }

    for (size_t y = 0; y < window.height; y++)
    {
        delete[] depthBuffer[y];
    }
    delete[] depthBuffer;
}

float** RasterRenderer::generateDepthBuffer(const std::vector<ModelTriangle>& triangles, const size_t width, const size_t height) const
{
    auto** depthBuffer = new float*[height];

    for (size_t y = 0; y < height; y++)
    {
        depthBuffer[y] = new float[width];
        for (size_t x = 0; x < width; x++)
        {
            depthBuffer[y][x] = 0.0f;
        }
    }

    const auto prePassUniform = FragmentData::PrePassUniform(depthBuffer, width, height);
    const auto placeholderColour = Colour();

    // Pre pass to get final closest depths using a much faster shader
    for (const auto& triangle : triangles)
    {
        const auto canvasTriangle = CanvasTriangle(
            projectVertexOntoCanvasPoint(triangle.vertices[0], width, height, camera.scale),
            projectVertexOntoCanvasPoint(triangle.vertices[1], width, height, camera.scale),
            projectVertexOntoCanvasPoint(triangle.vertices[2], width, height, camera.scale),
            placeholderColour
        );

        const std::array<FragmentData::PrePassData, 3> data = {
            FragmentData::PrePassData(canvasTriangle.vertices[0].depth),
            FragmentData::PrePassData(canvasTriangle.vertices[1].depth),
            FragmentData::PrePassData(canvasTriangle.vertices[2].depth)
        };

        drawTriangle<FragmentData::PrePassUniform, FragmentData::PrePassData>(
            canvasTriangle, prePassUniform, data, FragmentShaders::prePass);
    }

    return depthBuffer;
}

glm::vec3 RasterRenderer::applyCameraTransformation(const glm::vec3 vertex) const
{
    // Map model space to camera space
    glm::vec3 cameraVertexPosition = vertex - camera.position;

    cameraVertexPosition = cameraVertexPosition * camera.rotation;

    return cameraVertexPosition;
}

CanvasPoint RasterRenderer::projectVertexOntoCanvasPoint(const glm::vec3 vertex, const size_t width,
                                                          const size_t height, const float focalLength)
{
    float u = 0;
    float v = 0;
    float depth = 0;

    if (vertex.z != 0)
    {
        u = height * focalLength * (-vertex.x / vertex.z) + (static_cast<float>(width) / 2);
        v = height * focalLength * (vertex.y / vertex.z) + (static_cast<float>(height) / 2);

        // Negative due to positive z out the screen
        depth = -1.0f / vertex.z;
    }

    return {u, v, depth};
}
