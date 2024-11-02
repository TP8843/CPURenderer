//
// Created by Thomas Parr on 08/10/2024.
//

#include <vector>
#include <fstream>
#include <../../libs/sdw/Utils.h>
#include <../../libs/sdw/ModelTriangle.h>

#include "Model.h"
#include "../helper/Draw.h"
#include "../helper/Interpolation.h"
#include "../helper/MaterialMap.h"

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

            // Calculate normal for triangle
            const glm::vec3 normal = glm::cross(
                glm::cross(faceVertices.at(0), faceVertices.at(1)),
                faceVertices.at(2));

            if (hasTexture)
            {
                triangles.emplace_back(faceVertices[0], texturePoints[0],
                                        faceVertices[1], texturePoints[1],
                                        faceVertices[2], texturePoints[2],
                                        normal,
                                        currentMaterial);
            }
            else
            {
                triangles.emplace_back(faceVertices[0], faceVertices[1], faceVertices[2], normal, currentMaterial);
            }
        }
    }

    return Model(triangles, materialMap);
}

std::vector<ModelTriangle> Model::transformTriangles(const Camera& camera, std::vector<ModelTriangle> triangles)
{
    std::vector<ModelTriangle> newTriangles = std::vector<ModelTriangle>();

    for (ModelTriangle& triangle : triangles)
    {
        newTriangles.emplace_back(ModelTriangle(
            (triangle.vertices[0] - camera.position) * camera.rotation,
            triangle.texturePoints[0],
            (triangle.vertices[1] - camera.position) * camera.rotation,
            triangle.texturePoints[1],
            (triangle.vertices[2] - camera.position) * camera.rotation,
            triangle.texturePoints[2],
            triangle.normal,
            triangle.material));
    }

    return newTriangles;
}

std::vector<ModelTriangle> Model::clipTriangles(std::vector<ModelTriangle> triangles)
{
    std::vector<ModelTriangle> filteredTriangles = std::vector<ModelTriangle>(triangles);

    for (const auto& clippingPlane : getClippingPlanes())
    {
        std::vector<ModelTriangle> oldFilteredTriangles = filteredTriangles;
        filteredTriangles = std::vector<ModelTriangle>();

        for (ModelTriangle triangle: oldFilteredTriangles)
        {
            std::array<glm::vec3, 3> vertices = std::array<glm::vec3, 3>(triangle.vertices);
            std::array<TexturePoint, 3> texturePoints = std::array<TexturePoint, 3>(triangle.texturePoints);

            std::array<float, 3> distances = {
                clippingPlane.distanceRelativeToPlane(vertices[0]),
                clippingPlane.distanceRelativeToPlane(vertices[1]),
                clippingPlane.distanceRelativeToPlane(vertices[2])
            };

            // Sort distances into ascending order.
            // Changing order of vertices doesn't matter as TODO: normal calculated on import
            if (distances[0] > distances[1])
            {
                std::swap(distances[0], distances[1]);
                std::swap(vertices[0], vertices[1]);
                std::swap(texturePoints[0], texturePoints[1]);
            }

            if (distances[1] > distances[2])
            {
                std::swap(distances[1], distances[2]);
                std::swap(vertices[1], vertices[2]);
                std::swap(texturePoints[1], texturePoints[2]);
            }

            if (distances[0] > distances[1])
            {
                std::swap(distances[0], distances[1]);
                std::swap(vertices[0], vertices[1]);
                std::swap(texturePoints[0], texturePoints[1]);
            }

            if (distances[0] > 0.0f && distances[1] > 0.0f && distances[2] > 0.0f)
            {
                filteredTriangles.emplace_back(ModelTriangle(
                    vertices[0], texturePoints[0],
                    vertices[1], texturePoints[1],
                    vertices[2], texturePoints[2],
                    triangle.normal,
                    triangle.material));
            }
            // If one vertex outside clipping plane
            else if (distances[1] > 0.0f && distances[2] > 0.0f)
            {
                const float v1prop = clippingPlane.getIntersection(vertices[0], vertices[1]);
                const float v2prop = clippingPlane.getIntersection(vertices[0], vertices[2]);

                const glm::vec3 newV1Vertex = Interpolation::interpolate(vertices[0], vertices[1], v1prop);
                const TexturePoint newV1TexturePoint = Interpolation::interpolate(texturePoints[0], texturePoints[1], v1prop);

                const glm::vec3 newV2Vertex = Interpolation::interpolate(vertices[0], vertices[2], v2prop);
                const TexturePoint newV2TexturePoint = Interpolation::interpolate(texturePoints[0], texturePoints[2], v2prop);

                filteredTriangles.emplace_back(ModelTriangle(
                    newV1Vertex, newV1TexturePoint,
                    vertices[1], texturePoints[1],
                    vertices[2], texturePoints[2], triangle.normal, triangle.material));

                filteredTriangles.emplace_back(ModelTriangle(
                    newV2Vertex, newV2TexturePoint,
                    newV1Vertex, newV1TexturePoint,
                    vertices[2], texturePoints[2], triangle.normal, triangle.material));

            }
            // Check if two vertices outside plane
            else if (distances[2] > 0.0f)
            {
                const float v0prop = clippingPlane.getIntersection(vertices[0], vertices[2]);
                const float v1prop = clippingPlane.getIntersection(vertices[1], vertices[2]);

                filteredTriangles.emplace_back(ModelTriangle(
                    Interpolation::interpolate(vertices[0], vertices[2], v0prop),
                    Interpolation::interpolate(texturePoints[0], texturePoints[2], v0prop),
                    Interpolation::interpolate(vertices[1], vertices[2], v1prop),
                    Interpolation::interpolate(texturePoints[1], texturePoints[2], v1prop),
                    vertices[2],
                    texturePoints[2],
                    triangle.normal,
                    triangle.material));
            }
        }
    }

    return filteredTriangles;
}

std::vector<ModelTriangle> Model::getPreparedTriangles(const Camera& camera) const
{
    return clipTriangles(transformTriangles(camera, triangles));
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
