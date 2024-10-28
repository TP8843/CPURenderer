//
// Created by Thomas Parr on 08/10/2024.
//

#include <vector>
#include <fstream>
#include <unordered_map>
#include <../../libs/sdw/Utils.h>
#include <../../libs/sdw/ModelTriangle.h>

#include "Model.h"
#include "Draw.h"
#include "Interpolation.h"

Model::Model(const std::vector<ModelTriangle>& triangles) : triangles(triangles)
{}

Model Model::import(const char* objectPath)
{
    std::string text;
    std::ifstream ObjectFile(objectPath);

    std::unordered_map<std::string, Material> materialMap;
    Material currentMaterial;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> vertexTextures;
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

        // Texture Points
        if (tokens.at(0) == "vt")
        {
            const auto vt = glm::vec2(
                std::stof(tokens.at(1)),
                std::stof(tokens.at(2)));

            vertexTextures.emplace_back(vt);
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
            std::vector<TexturePoint> texturePoints;
            bool hasTexture = false;

            for (int i = 1; i <= 3; i++)
            {
                auto vertexTokens = split(tokens.at(i), '/');

                // Pull correct vertex from read in vertices
                faceVertices.push_back(vertices[std::stoi(vertexTokens.at(0)) - 1]);

                // If vertex has material data
                if (!vertexTokens.at(1).empty())
                {
                    hasTexture = true;
                    const auto point = vertexTextures[std::stoi(vertexTokens.at(1)) - 1];

                    texturePoints.emplace_back(point.x * currentMaterial.textureMap.width, point.y * currentMaterial.textureMap.height);
                }
            }

            if (hasTexture)
            {
                triangles.emplace_back(faceVertices[0], texturePoints[0],
                                        faceVertices[1], texturePoints[1],
                                        faceVertices[2], texturePoints[2],
                                        currentMaterial);
            }
            else
            {
                triangles.emplace_back(faceVertices[0], faceVertices[1], faceVertices[2], currentMaterial);
            }
        }
    }

    return Model(triangles);
}

std::unordered_map<std::string, Material> Model::importMaterials(const std::string &path)
{
    std::ifstream MaterialFile(path);
    std::string line;
    std::string currentMaterial;
    std::unordered_map<std::string, Material> materialMap;

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

            materialMap[currentMaterial] = Material(currentMaterial, colourValue);
            std::cout << "Material " << currentMaterial << " added colour: " << materialMap[currentMaterial].colour << std::endl;
        }

        if (tokens.at(0) == "map_Kd")
        {
            const auto& texturePath = tokens.at(1);
            const auto textureMap = TextureMap(texturePath);

            materialMap[currentMaterial].textureMap = textureMap;
            materialMap[currentMaterial].hasTexture = true;
        }
    }

    return materialMap;
}
