//
// Created by Thomas Parr on 08/10/2024.
//

#include "Model.h"

#include <ModelTriangle.h>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <Utils.h>

#include "Draw.h"
#include "Interpolation.h"
#include "Interpolation.h"
#include "Interpolation.h"
#include "Interpolation.h"
#include "Interpolation.h"
#include "Interpolation.h"

Model::Model(const std::vector<ModelTriangle>& triangles) : triangles(triangles)
{}

Model Model::import(const char* objectPath)
{
    std::string text;
    std::ifstream ObjectFile(objectPath);

    std::unordered_map<std::string, Colour> materialMap;
    Colour currentMaterial;
    std::vector<glm::vec3> vertices;
    std::vector<ModelTriangle> triangles;

    while(getline(ObjectFile, text))
    {
        // Split on all spaces
        const auto tokens = split(text, ' ');

        // Import materials
        if (tokens.at(0) == "mtllib")
        {
            materialMap = importMaterials(tokens[1]);
        }

        // Vertex
        if (tokens.at(0) == "v")
        {
            const auto v = glm::vec3(
                std::stof(tokens.at(1)),
                std::stof(tokens.at(2)),
                std::stof(tokens.at(3)));

            vertices.emplace_back(v);
        }

        // Material
        if (tokens.at(0) == "usemtl")
        {
            currentMaterial = materialMap[tokens.at(1)];
        }

        // Face
        if (tokens.at(0) == "f")
        {
            std::vector<glm::vec3> faceVertices;

            for (int i = 1; i <= 3; i++)
            {
                // Pull correct vertex from read in vertices
                faceVertices.push_back(vertices[std::stoi(tokens.at(i)) - 1]);
            }

            triangles.emplace_back(faceVertices[0], faceVertices[1], faceVertices[2], currentMaterial);
        }
    }

    return Model(triangles);
}

CanvasPoint Model::projectVertexOntoCanvasPoint(const glm::vec3 cameraPosition, const float focalLength, const float imagePlaneScaling, const glm::vec3 vertexPosition, const glm::vec2 canvasSize)
{
    // Map model space to camera space
    glm::vec3 cameraVertexPosition = vertexPosition - cameraPosition;

    float u = imagePlaneScaling * focalLength * (- cameraVertexPosition.x / cameraVertexPosition.z) + (canvasSize.x / 2);
    float v = imagePlaneScaling * focalLength * (cameraVertexPosition.y / cameraVertexPosition.z) + (canvasSize.y / 2);

    return {u, v};
}

void Model::pointcloudRender(DrawingWindow &window, glm::vec3 cameraPosition, float focalLength, float imagePlaneScaling)
{
    const auto canvasSize = glm::vec2(window.width, window.height);

    for (const auto& triangle: triangles)
    {
        for (auto vertex: triangle.vertices)
        {
            auto mappedVertex = projectVertexOntoCanvasPoint(cameraPosition, focalLength, imagePlaneScaling, vertex, canvasSize);

            window.setPixelColour(mappedVertex.x, mappedVertex.y, 0xFFFFFFFF);
        }
    }
}

void Model::wireframeRender(DrawingWindow &window, glm::vec3 cameraPosition, float focalLength, float imagePlaneScaling)
{
    const auto canvasSize = glm::vec2(window.width, window.height);

    for (const auto& triangle: triangles)
    {
        std::vector<CanvasPoint> mappedVertices;

        for (auto vertex: triangle.vertices)
        {
            mappedVertices.push_back(projectVertexOntoCanvasPoint(cameraPosition, focalLength, imagePlaneScaling, vertex, canvasSize));
        }

        Draw::drawStrokedTriangle(window, CanvasTriangle(mappedVertices[0], mappedVertices[1], mappedVertices[2]), Colour(255, 255, 255));
    }
}

void Model::rasterRender(DrawingWindow &window, glm::vec3 cameraPosition, float focalLength, float imagePlaneScaling)
{
    const auto canvasSize = glm::vec2(window.width, window.height);

    for (const auto& triangle: triangles)
    {
        std::vector<CanvasPoint> mappedVertices;

        for (auto vertex: triangle.vertices)
        {
            mappedVertices.push_back(projectVertexOntoCanvasPoint(cameraPosition, focalLength, imagePlaneScaling, vertex, canvasSize));
        }

        Draw::drawFilledTriangle(window, CanvasTriangle(mappedVertices[0], mappedVertices[1], mappedVertices[2]), triangle.colour);
    }
}

std::unordered_map<std::string, Colour> Model::importMaterials(const std::string &path)
{
    std::ifstream MaterialFile(path);
    std::string line;
    std::string currentMaterial;
    std::unordered_map<std::string, Colour> materialMap;

    while(getline(MaterialFile, line))
    {
        const auto tokens = split(line, ' ');

        if (tokens.at(0) == "newmtl")
        {
            currentMaterial = tokens.at(1);
        }

        if (tokens.at(0) == "Kd")
        {
            const auto colourValue = Colour(
                std::stof(tokens.at(1)) * 255,
                std::stof(tokens.at(2)) * 255,
                std::stof(tokens.at(3)) * 255);

            materialMap[currentMaterial] = colourValue;
            std::cout << "Material " << currentMaterial << " added: " << materialMap[currentMaterial] << std::endl;
        }
    }

    return materialMap;
}
