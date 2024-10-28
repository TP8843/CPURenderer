//
// Created by Thomas Parr on 28/10/2024.
//

#include "RasterRenderer2.h"

#include "helper/Camera.h"
#include "helper/Camera.h"
#include "helper/Draw.h"
#include "shaders/FragmentShaders.h"
#include "shaders/FragmentData.h"

RasterRenderer2::RasterRenderer2(Model& model, Camera& camera) :
        model(model), camera(camera)
{}

void RasterRenderer2::pointCloudRender(DrawingWindow& window) const
{
    for (const auto& triangle : model.triangles)
    {
        for (const auto vertex : triangle.vertices)
        {
            const auto mappedVertex = projectVertexOntoCanvasPoint(vertex, window.width, window.height);

            window.setPixelColour(mappedVertex.x, mappedVertex.y, 0xFFFFFFFF);
        }
    }
}

void RasterRenderer2::wireframeRender(DrawingWindow& window) const
{
    for (const auto& triangle : model.triangles)
    {
        auto mappedVertices = std::vector<CanvasPoint>();

        for (const auto vertex : triangle.vertices)
        {
            mappedVertices.push_back(projectVertexOntoCanvasPoint(vertex, window.width, window.height));
        }

        Draw::drawStrokedTriangle(window, CanvasTriangle(mappedVertices[0], mappedVertices[1], mappedVertices[2], Colour(255, 255, 255)));
    }
}

void RasterRenderer2::rasterRender(DrawingWindow& window) const
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

    for (const auto &triangle : model.triangles)
    {
        auto mappedVertices = std::vector<CanvasPoint>();
        auto fragmentData = std::vector<FragmentData::FilledData>();

        CanvasPoint v1 = projectVertexOntoCanvasPoint(triangle.vertices[0], window.width, window.height);
        CanvasPoint v2 = projectVertexOntoCanvasPoint(triangle.vertices[1], window.width, window.height);
        CanvasPoint v3 = projectVertexOntoCanvasPoint(triangle.vertices[2], window.width, window.height);

        CanvasTriangle canvasTriangle = CanvasTriangle(v1, v2, v3, triangle.material.colour);

        if (!triangle.material.hasTexture)
        {
            FragmentData::FilledDataUniform uniform = FragmentData::FilledDataUniform();

            FragmentData::FilledData d1 = { triangle.material.colour, glm::vec3(1.0f, 0.0f, 0.0f), v1.depth };
            FragmentData::FilledData d2 = { triangle.material.colour, glm::vec3(0.0f, 1.0f, 0.0f), v2.depth };
            FragmentData::FilledData d3 = { triangle.material.colour, glm::vec3(0.0f, 0.0f, 1.0f), v3.depth };

            std::array<FragmentData::FilledData, 3> data = { d1, d2, d3 };

            drawTriangle<FragmentData::FilledDataUniform, FragmentData::FilledData>(window, canvasTriangle, depthBuffer, uniform, data, FragmentShaders::filled );
        }
        else
        {
            FragmentData::TextureDataUniform uniform = FragmentData::TextureDataUniform(triangle.material.textureMap);

            FragmentData::TextureData d1 = { triangle.material.colour, triangle.texturePoints[0] * v1.depth, glm::vec3(1.0f, 0.0f, 0.0f), v1.depth };
            FragmentData::TextureData d2 = { triangle.material.colour, triangle.texturePoints[1] * v2.depth, glm::vec3(0.0f, 1.0f, 0.0f), v2.depth };
            FragmentData::TextureData d3 = { triangle.material.colour, triangle.texturePoints[2] * v3.depth, glm::vec3(0.0f, 0.0f, 1.0f), v3.depth };

            std::array<FragmentData::TextureData, 3> data = { d1, d2, d3 };

            drawTriangle<FragmentData::TextureDataUniform, FragmentData::TextureData>(window, canvasTriangle, depthBuffer, uniform, data, FragmentShaders::material );
        }
    }

    for (int y = 0; y < window.height; y++)
    {
        delete[] depthBuffer[y];
    }
    delete[] depthBuffer;
}

float** RasterRenderer2::createDepthBuffer(const int width, const int height)
{
    auto** depthBuffer = new float*[height];

    for (int y = 0; y < height; y++)
    {
        depthBuffer[y] = new float[width];
        for (int x = 0; x < width; x++)
        {
            depthBuffer[y][x] = 0.0f;
        }
    }

    return depthBuffer;
}

CanvasPoint RasterRenderer2::projectVertexOntoCanvasPoint(const glm::vec3 vertex, const size_t width, const size_t height) const
{
    // Map model space to camera space
    glm::vec3 cameraVertexPosition = vertex - camera.position;

    cameraVertexPosition = cameraVertexPosition * camera.rotation;
    float u = 0;
    float v = 0;

    float depth = 0;

    if (cameraVertexPosition.z != 0)
    {
        u = camera.imagePlaneScaling * camera.focalLength * (-cameraVertexPosition.x / cameraVertexPosition.z) + (width / 2);
        v = camera.imagePlaneScaling * camera.focalLength * (cameraVertexPosition.y / cameraVertexPosition.z) + (height / 2);

        // Negative due to positive z out the screen
        depth = -1.0f / cameraVertexPosition.z;
    }


    return {u, v, depth};
}
