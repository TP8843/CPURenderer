//
// Created by Thomas Parr on 08/10/2024.
//

#include <vector>
#include <fstream>
#include <../../libs/sdw/Utils.h>
#include <../../libs/sdw/ModelTriangle.h>

#include "Model.h"
#include "Draw.h"
#include "Interpolation.h"
#include "MaterialMap.h"

Model::Model(const std::vector<ModelTriangle>& triangles, const MaterialMap& materials) :
    triangles(triangles), materials(materials)
{}

Model Model::import(const char* objectPath)
{
    std::string text;
    std::ifstream ObjectFile(objectPath);

    MaterialMap materialMap;
    std::string currentMaterial;
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
            currentMaterial = tokens.at(1);
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

                    texturePoints.emplace_back(
                        point.x * materialMap.getMaterial(currentMaterial).getTextureWidth(),
                        point.y * materialMap.getMaterial(currentMaterial).getTextureHeight());
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

    return Model(triangles, materialMap);
}

MaterialMap Model::importMaterials(const std::string &path)
{
    std::ifstream MaterialFile(path);
    std::string line;
    std::string currentMaterialName;

    bool materialToStore = false;

    bool hasTexture = false;
    TextureMap currentTexture;

    bool hasColour = false;
    Colour currentColour;

    MaterialMap materialMap = MaterialMap();

    while(getline(MaterialFile, line))
    {
        const auto tokens = split(line, ' ');

        if (tokens.at(0) == "newmtl")
        {
            if (materialToStore)
            {

                if (hasTexture && hasColour)
                {
                    std::cout << "Material " << currentMaterialName << " added colour + texture: " << currentColour << std::endl;

                    materialMap.addMaterial(currentMaterialName, Material(currentColour, currentTexture));
                }
                else if (hasTexture)
                {
                    materialMap.addMaterial(currentMaterialName, Material(Colour(255, 255, 255), currentTexture));
                }
                else if (hasColour)
                {
                    std::cout << "Material " << currentMaterialName << " added colour: " << currentColour << std::endl;
                    materialMap.addMaterial(currentMaterialName, Material(currentColour));
                }
            }

            materialToStore = false;
            hasColour = false;
            hasTexture = false;

            currentMaterialName = tokens.at(1);
        }

        if (tokens.at(0) == "Kd")
        {
            materialToStore = true;

            const auto colourValue = Colour(
                std::stof(tokens.at(1)) * 255,
                std::stof(tokens.at(2)) * 255,
                std::stof(tokens.at(3)) * 255);

            hasColour = true;
            currentColour = colourValue;
        }

        if (tokens.at(0) == "map_Kd")
        {
            materialToStore = true;

            const auto& texturePath = tokens.at(1);
            const auto textureMap = TextureMap(texturePath);

            hasTexture = true;
            currentTexture = textureMap;
        }
    }

    if (materialToStore)
    {
        if (hasTexture && hasColour)
        {
            materialMap.addMaterial(currentMaterialName, Material(currentColour, currentTexture));
        }
        else if (hasTexture)
        {
            materialMap.addMaterial(currentMaterialName, Material(Colour(255, 255, 255), currentTexture));
        }
        else if (hasColour)
        {
            materialMap.addMaterial(currentMaterialName, Material(currentColour));
        }
    }

    return materialMap;
}
