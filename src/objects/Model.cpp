#include <vector>
#include <fstream>
#include <../../libs/sdw/Utils.h>
#include <../../libs/sdw/ModelTriangle.h>

#include "Model.h"
#include "../helper/Draw.h"
#include "../helper/Interpolation.h"
#include "./materials/MaterialMap.h"

Model::Model(const std::vector<ModelTriangle>& triangles, const MaterialMap& materials) :
    materials(materials), triangles(triangles)
{
}

Model Model::import(const char* objectPath, const float scale)
{
    std::string text;
    std::ifstream ObjectFile(objectPath);

    MaterialMap materialMap = MaterialMap();
    materialMap.addMaterial("Backup", Material(Colour(255, 255, 255), FLAT, 1.0f));
    std::string currentMaterial = "Backup";

    std::vector<glm::vec3> vertices;

    // Stores the total of the adjacent face normals and the number of adjacent faces (vertex normal total, number of normals)
    std::vector<std::pair<glm::vec3, float>> vertexNormalTotals;

    // The vertices for each triangle
    std::vector<std::vector<int>> triangleVertexIndices;

    std::vector<glm::vec2> vertexTextures;
    std::vector<ModelTriangle> triangles;

    while (getline(ObjectFile, text))
    {
        // Split on all spaces
        const auto tokens = split(text, ' ');

        // Import materials
        if (tokens.at(0) == "mtllib")
        {
            materialMap = importMaterials(materialMap, tokens[1]);
        }

        // Vertex
        if (tokens.at(0) == "v")
        {
            const auto v = glm::vec3(
                scale * std::stof(tokens.at(1)),
                scale * std::stof(tokens.at(2)),
                scale * std::stof(tokens.at(3)));

            vertices.emplace_back(v);

            vertexNormalTotals.emplace_back(std::pair<glm::vec3, float>(glm::vec3(0, 0, 0), 0));
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
            std::vector<int> vertexIndices;
            std::vector<TexturePoint> texturePoints;
            bool hasTexture = false;

            for (int i = 1; i <= 3; i++)
            {
                auto vertexTokens = split(tokens.at(i), '/');

                // Pull correct vertex from read in vertices
                faceVertices.push_back(vertices[std::stoi(vertexTokens.at(0)) - 1]);

                vertexIndices.push_back(std::stoi(vertexTokens.at(0)) - 1);

                // If vertex has material data
                if (vertexTokens.size() > 1 && !vertexTokens.at(1).empty())
                {
                    hasTexture = true;
                    const auto point = vertexTextures[std::stoi(vertexTokens.at(1)) - 1];

                    texturePoints.emplace_back(
                        point.x * materialMap.getMaterial(currentMaterial).getTextureWidth(),
                        point.y * materialMap.getMaterial(currentMaterial).getTextureHeight());
                }
            }

            triangleVertexIndices.push_back(vertexIndices);

            // Calculate normal for triangle
            const glm::vec3 normal = glm::normalize(glm::cross(
                faceVertices.at(0) - faceVertices.at(1),
                faceVertices.at(2) - faceVertices.at(0)));

            for (const auto& vertex : vertexIndices)
            {
                const auto vertexNormalTotal = vertexNormalTotals.at(vertex);
                vertexNormalTotals.at(vertex) = std::pair<glm::vec3, float>(
                    vertexNormalTotal.first + normal,
                    vertexNormalTotal.second + 1);
            }

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

    // Add vertex normals to all triangles
    for (int i = 0; i < triangleVertexIndices.size(); i++)
    {
        std::vector<int> vertexIndices = triangleVertexIndices.at(i);

        for (int j = 0; j < vertexIndices.size(); j++)
        {
            const glm::vec3 vertexNormal = glm::normalize(
                vertexNormalTotals.at(vertexIndices.at(j)).first / vertexNormalTotals.at(vertexIndices.at(j)).second);

            triangles[i].vertexNormals[j] = vertexNormal;
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
            triangle.vertexNormals[0] * camera.rotation,
            (triangle.vertices[1] - camera.position) * camera.rotation,
            triangle.texturePoints[1],
            triangle.vertexNormals[1] * camera.rotation,
            (triangle.vertices[2] - camera.position) * camera.rotation,
            triangle.texturePoints[2],
            triangle.vertexNormals[2] * camera.rotation,
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

        for (ModelTriangle triangle : oldFilteredTriangles)
        {
            std::array<glm::vec3, 3> vertices = std::array<glm::vec3, 3>(triangle.vertices);
            std::array<TexturePoint, 3> texturePoints = std::array<TexturePoint, 3>(triangle.texturePoints);
            std::array<glm::vec3, 3> vertexNormals = std::array<glm::vec3, 3>(triangle.vertexNormals);

            std::array<float, 3> distances = {
                clippingPlane.distanceRelativeToPlane(vertices[0]),
                clippingPlane.distanceRelativeToPlane(vertices[1]),
                clippingPlane.distanceRelativeToPlane(vertices[2])
            };

            // Sort distances into ascending order.
            // Changing order of vertices doesn't matter as normal calculated on import
            if (distances[0] > distances[1])
            {
                std::swap(distances[0], distances[1]);
                std::swap(vertices[0], vertices[1]);
                std::swap(texturePoints[0], texturePoints[1]);
                std::swap(vertexNormals[0], vertexNormals[1]);
            }

            if (distances[1] > distances[2])
            {
                std::swap(distances[1], distances[2]);
                std::swap(vertices[1], vertices[2]);
                std::swap(texturePoints[1], texturePoints[2]);
                std::swap(vertexNormals[1], vertexNormals[2]);
            }

            if (distances[0] > distances[1])
            {
                std::swap(distances[0], distances[1]);
                std::swap(vertices[0], vertices[1]);
                std::swap(texturePoints[0], texturePoints[1]);
                std::swap(vertexNormals[0], vertexNormals[1]);
            }

            if (distances[0] > 0.0f && distances[1] > 0.0f && distances[2] > 0.0f)
            {
                filteredTriangles.emplace_back(ModelTriangle(
                    vertices[0], texturePoints[0], vertexNormals[0],
                    vertices[1], texturePoints[1], vertexNormals[1],
                    vertices[2], texturePoints[2], vertexNormals[2],
                    triangle.normal,
                    triangle.material));
            }
            // If one vertex outside clipping plane
            else if (distances[1] > 0.0f && distances[2] > 0.0f)
            {
                const float v1prop = clippingPlane.getIntersection(vertices[0], vertices[1]);
                const float v2prop = clippingPlane.getIntersection(vertices[0], vertices[2]);

                const glm::vec3 newV1Vertex = Interpolation::interpolate(vertices[0], vertices[1], v1prop);

                const TexturePoint newV1TexturePoint = Interpolation::interpolate(
                    texturePoints[0], texturePoints[1], v1prop);

                const glm::vec3 newV1VertexNormal = Interpolation::interpolate(
                    vertexNormals[0], vertexNormals[1], v1prop);

                const glm::vec3 newV2Vertex = Interpolation::interpolate(vertices[0], vertices[2], v2prop);

                const TexturePoint newV2TexturePoint = Interpolation::interpolate(
                    texturePoints[0], texturePoints[2], v2prop);

                const glm::vec3 newV2VertexNormal = Interpolation::interpolate(
                    vertexNormals[0], vertexNormals[2], v2prop);

                filteredTriangles.emplace_back(ModelTriangle(
                    newV1Vertex, newV1TexturePoint, newV1VertexNormal,
                    vertices[1], texturePoints[1], vertexNormals[1],
                    vertices[2], texturePoints[2], vertexNormals[2],
                    triangle.normal, triangle.material));

                filteredTriangles.emplace_back(ModelTriangle(
                    newV2Vertex, newV2TexturePoint, newV2VertexNormal,
                    newV1Vertex, newV1TexturePoint, newV1VertexNormal,
                    vertices[2], texturePoints[2], vertexNormals[2],
                    triangle.normal, triangle.material));
            }
            // Check if two vertices outside plane
            else if (distances[2] > 0.0f)
            {
                const float v0prop = clippingPlane.getIntersection(vertices[0], vertices[2]);
                const float v1prop = clippingPlane.getIntersection(vertices[1], vertices[2]);

                filteredTriangles.emplace_back(ModelTriangle(
                    Interpolation::interpolate(vertices[0], vertices[2], v0prop),
                    Interpolation::interpolate(texturePoints[0], texturePoints[2], v0prop),
                    Interpolation::interpolate(vertexNormals[0], vertexNormals[2], v0prop),
                    Interpolation::interpolate(vertices[1], vertices[2], v1prop),
                    Interpolation::interpolate(texturePoints[1], texturePoints[2], v1prop),
                    Interpolation::interpolate(vertexNormals[1], vertexNormals[2], v1prop),
                    vertices[2],
                    texturePoints[2],
                    vertexNormals[2],
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

MaterialMap Model::importMaterials(MaterialMap& materialMap, const std::string& path)
{
    std::ifstream MaterialFile(path);
    std::string line;
    std::string currentMaterialName;

    bool materialToStore = false;

    bool hasTexture = false;
    TextureMap currentTexture;

    bool hasColour = false;
    Colour currentColour;

    IlluminationModel illuminationModel = FLAT;

    float specularStrength = 1.0f;

    while (getline(MaterialFile, line))
    {
        const auto tokens = split(line, ' ');

        // Defines a new material
        if (tokens.at(0) == "newmtl")
        {
            if (materialToStore)
            {
                if (hasTexture && hasColour)
                {
                    materialMap.addMaterial(currentMaterialName, Material(currentColour, illuminationModel, specularStrength, currentTexture));
                }
                else if (hasTexture)
                {
                    materialMap.addMaterial(currentMaterialName, Material(Colour(255, 255, 255), illuminationModel, specularStrength, currentTexture));
                }
                else if (hasColour)
                {
                    materialMap.addMaterial(currentMaterialName, Material(currentColour, illuminationModel, specularStrength));
                }
            }

            materialToStore = false;
            hasColour = false;
            hasTexture = false;

            currentMaterialName = tokens.at(1);
            illuminationModel = FLAT;
            specularStrength = 1.0f;
        }

        // Colour for diffuse lighting
        if (tokens.at(0) == "Kd")
        {
            materialToStore = true;

            const auto colourValue = Colour(
                static_cast<int>(glm::round(std::stof(tokens.at(1)) * 255.0f)),
                static_cast<int>(glm::round(std::stof(tokens.at(2)) * 255.0f)),
                static_cast<int>(glm::round(std::stof(tokens.at(3)) * 255.0f)));

            hasColour = true;
            currentColour = colourValue;
        }

        // Strength of specular highlights
        if (tokens.at(0) == "Ns")
        {
            specularStrength = std::stof(tokens.at(1));
        }

        // Location of texture map
        if (tokens.at(0) == "map_Kd")
        {
            materialToStore = true;

            const auto& texturePath = tokens.at(1);
            const auto textureMap = TextureMap(texturePath);

            hasTexture = true;
            currentTexture = textureMap;
        }

        // Illumination type (UNSHADED, FLAT, PHONG)
        if (tokens.at(0) == "illum")
        {
            switch (std::stoi(tokens.at(1)))
            {
                case 0: illuminationModel = UNSHADED;
                case 1: illuminationModel = FLAT;
                case 2: illuminationModel = PHONG;
                default: illuminationModel = FLAT;
            }
        }
    }

    if (materialToStore)
    {
        if (hasTexture && hasColour)
        {
            materialMap.addMaterial(currentMaterialName, Material(currentColour, illuminationModel, specularStrength, currentTexture));
        }
        else if (hasTexture)
        {
            materialMap.addMaterial(currentMaterialName, Material(Colour(255, 255, 255), illuminationModel, specularStrength, currentTexture));
        }
        else if (hasColour)
        {
            materialMap.addMaterial(currentMaterialName, Material(currentColour, illuminationModel, specularStrength));
        }
    }

    return materialMap;
}
