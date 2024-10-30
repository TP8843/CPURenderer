//
// Created by Thomas Parr on 28/10/2024.
//

#include "RasterRenderer2.h"

#include "../helper/Camera.h"
#include "../helper/Draw.h"
#include "../shaders/FragmentShaders.h"
#include "../shaders/FragmentData.h"

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
        const std::array<glm::vec3, 3> transformedVertices = {
            applyCameraTransformation(triangle.vertices[0]),
            applyCameraTransformation(triangle.vertices[1]),
            applyCameraTransformation(triangle.vertices[2]),
        };

        if (transformedVertices[0].z > -1.0f &&
            transformedVertices[1].z > -1.0f &&
            transformedVertices[2].z > -1.0f)
        {
            continue;
        }

        auto mappedVertices = std::vector<CanvasPoint>();
        auto fragmentData = std::vector<FragmentData::FilledData>();
        const auto material = model.materials.getMaterial(triangle.material);

        CanvasPoint v1 = projectVertexOntoCanvasPoint(triangle.vertices[0], window.width, window.height);
        CanvasPoint v2 = projectVertexOntoCanvasPoint(triangle.vertices[1], window.width, window.height);
        CanvasPoint v3 = projectVertexOntoCanvasPoint(triangle.vertices[2], window.width, window.height);

        // Placeholder colour to allow original rasterer to work
        CanvasTriangle canvasTriangle = CanvasTriangle(v1, v2, v3, Colour(255, 255, 255));

        if (!material.hasTexture())
        {
            FragmentData::FilledDataUniform uniform = FragmentData::FilledDataUniform(window, depthBuffer);

            FragmentData::FilledData d1 = { material.getColour(), glm::vec3(1.0f, 0.0f, 0.0f), v1.depth };
            FragmentData::FilledData d2 = { material.getColour(), glm::vec3(0.0f, 1.0f, 0.0f), v2.depth };
            FragmentData::FilledData d3 = { material.getColour(), glm::vec3(0.0f, 0.0f, 1.0f), v3.depth };

            std::array<FragmentData::FilledData, 3> data = { d1, d2, d3 };

            drawTriangle<FragmentData::FilledDataUniform, FragmentData::FilledData>(canvasTriangle, uniform, data, FragmentShaders::filled );
        }
        else
        {
            FragmentData::TextureDataUniform uniform = FragmentData::TextureDataUniform(window, depthBuffer, material);

            FragmentData::TextureData d1 = { material.getColour(), triangle.texturePoints[0] * v1.depth, glm::vec3(1.0f, 0.0f, 0.0f), v1.depth };
            FragmentData::TextureData d2 = { material.getColour(), triangle.texturePoints[1] * v2.depth, glm::vec3(0.0f, 1.0f, 0.0f), v2.depth };
            FragmentData::TextureData d3 = { material.getColour(), triangle.texturePoints[2] * v3.depth, glm::vec3(0.0f, 0.0f, 1.0f), v3.depth };

            std::array<FragmentData::TextureData, 3> data = { d1, d2, d3 };

            drawTriangle<FragmentData::TextureDataUniform, FragmentData::TextureData>(canvasTriangle, uniform, data, FragmentShaders::material );
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

glm::vec3 RasterRenderer2::applyCameraTransformation(const glm::vec3 vertex) const
{
    // Map model space to camera space
    glm::vec3 cameraVertexPosition = vertex - camera.position;

    cameraVertexPosition = cameraVertexPosition * camera.rotation;

    return cameraVertexPosition;
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
