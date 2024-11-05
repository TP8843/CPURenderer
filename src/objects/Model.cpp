#include <vector>
#include <fstream>
#include <../../libs/sdw/Utils.h>
#include <../../libs/sdw/ModelTriangle.h>

#include "Model.h"
#include "../helper/Draw.h"
#include "../helper/Interpolation.h"
#include "../helper/MaterialMap.h"

Model::Model(const std::vector<ModelTriangle>& triangles, const MaterialMap& materials, float scale) :
    materials(materials), triangles(triangles), scale(scale)
{
}

Model Model::import(const char* objectPath, const float scale)
{
    std::string text;
    std::ifstream ObjectFile(objectPath);

    MaterialMap materialMap = MaterialMap();
    materialMap.addMaterial("Backup", Material(Colour(255, 255, 255)));
    std::string currentMaterial = "Backup";

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> vertexTexturePoints;

    // Stores the total of the adjacent face normals and the number of adjacent faces
    std::vector<std::pair<glm::vec3, float>> vertexNormalTotals;
    std::vector<std::vector<int>> triangleVertexIndices;

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
            vertices.emplace_back(
                std::stof(tokens.at(1)),
                std::stof(tokens.at(2)),
                std::stof(tokens.at(3)));

            vertexNormalTotals.emplace_back(glm::vec3(0, 0, 0), 0);
        }

        // Texture Points
        if (tokens.at(0) == "vt")
        {
            const glm::vec2 vt = glm::vec2(
                std::stof(tokens.at(1)),
                std::stof(tokens.at(2)));

            vertexTexturePoints.emplace_back(vt);
        }

        // Material
        if (tokens.at(0) == "usemtl")
        {
            currentMaterial = tokens.at(1);
        }

        // Face
        if (tokens.at(0) == "f")
        {
            std::vector<ModelPoint> faceVertices;
            std::vector<int> vertexIndices;
            std::vector<TexturePoint> texturePoints;

            for (int i = 1; i <= 3; i++)
            {
                const auto vertexTokens = split(tokens.at(i), '/');

                // Pull correct vertex from read in vertices
                const int vertexIndex = std::stoi(vertexTokens.at(0)) - 1;

                vertexIndices.push_back(vertexIndex);

                ModelPoint vertex = ModelPoint(vertices.at(vertexIndex));

                // If vertex has material data
                if (vertexTokens.size() > 1 && !vertexTokens.at(1).empty())
                {
                    const glm::vec2 point = vertexTexturePoints.at(std::stoi(vertexTokens.at(1)) - 1);

                    vertex.texturePoint = TexturePoint(
                        point.x * static_cast<float>(materialMap.getMaterial(currentMaterial).getTextureWidth()),
                        point.y * static_cast<float>(materialMap.getMaterial(currentMaterial).getTextureHeight()));
                }

                faceVertices.push_back(vertex);
            }

            triangleVertexIndices.push_back(vertexIndices);

            // Calculate normal for triangle
            const glm::vec3 normal = glm::normalize(glm::cross(
                faceVertices.at(0).position - faceVertices.at(1).position,
                faceVertices.at(2).position - faceVertices.at(0).position));

            // Ensure that triangle is counted for average for vertex normals
            for (const auto& vertex : vertexIndices)
            {
                const auto vertexNormalTotal = vertexNormalTotals.at(vertex);
                vertexNormalTotals.at(vertex) = std::pair<glm::vec3, float>(
                    vertexNormalTotal.first + normal,
                    vertexNormalTotal.second + 1);
            }

            triangles.emplace_back(faceVertices[0], faceVertices[1], faceVertices[2], normal, currentMaterial);
        }
    }

    // Add vertex normals to all triangles
    for (size_t i = 0; i < triangleVertexIndices.size(); i++)
    {
        std::vector<int> vertexIndices = triangleVertexIndices.at(i);

        for (int j = 0; j < vertexIndices.size(); j++)
        {
            // Gets average of adjacent face normals
            const glm::vec3 vertexNormal = glm::normalize(
                vertexNormalTotals.at(vertexIndices.at(j)).first / vertexNormalTotals.at(vertexIndices.at(j)).second);

            triangles[i].vertices[j].normal = vertexNormal;
        }
    }

    return Model(triangles, materialMap, scale);
}

std::vector<ModelTriangle> Model::transformTriangles() const
{
    std::vector<ModelTriangle> newTriangles;

    for (const ModelTriangle& triangle : triangles)
    {
        const ModelPoint v0 = ModelPoint(
            triangle.vertices.at(0).position * scale,
            triangle.vertices.at(0).texturePoint,
            triangle.normal);

        const ModelPoint v1 = ModelPoint(
            triangle.vertices.at(1).position * scale,
            triangle.vertices.at(1).texturePoint,
            triangle.normal);

        const ModelPoint v2 = ModelPoint(
            triangle.vertices.at(2).position * scale,
            triangle.vertices.at(2).texturePoint,
            triangle.normal);

        newTriangles.emplace_back(v0, v1, v2, triangle.normal, triangle.material);
    }

    return newTriangles;
}

std::vector<ModelTriangle> Model::toCameraSpace(const Camera& camera, std::vector<ModelTriangle> triangles)
{
    std::vector<ModelTriangle> newTriangles = std::vector<ModelTriangle>();

    for (ModelTriangle& triangle : triangles)
    {
        const ModelPoint v0 = ModelPoint(
            (triangle.vertices.at(0).position - camera.position) * camera.rotation,
            triangle.vertices.at(0).texturePoint,
            triangle.vertices.at(0).normal * camera.rotation);

        const ModelPoint v1 = ModelPoint(
            (triangle.vertices.at(1).position - camera.position) * camera.rotation,
            triangle.vertices.at(1).texturePoint,
            triangle.vertices.at(1).normal * camera.rotation);

        const ModelPoint v2 = ModelPoint(
            (triangle.vertices.at(2).position - camera.position) * camera.rotation,
            triangle.vertices.at(2).texturePoint,
            triangle.vertices.at(2).normal * camera.rotation);

        newTriangles.emplace_back(v0, v1, v2, triangle.normal, triangle.material);
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
            std::array<ModelPoint, 3> vertices = std::array<ModelPoint, 3>(triangle.vertices);

            std::array<float, 3> distances = {
                clippingPlane.distanceRelativeToPlane(vertices[0].position),
                clippingPlane.distanceRelativeToPlane(vertices[1].position),
                clippingPlane.distanceRelativeToPlane(vertices[2].position)
            };

            // Sort distances into ascending order.
            // Changing order of vertices doesn't matter as normal calculated on import
            if (distances[0] > distances[1])
            {
                std::swap(distances[0], distances[1]);
                std::swap(vertices[0], vertices[1]);
            }

            if (distances[1] > distances[2])
            {
                std::swap(distances[1], distances[2]);
                std::swap(vertices[1], vertices[2]);
            }

            if (distances[0] > distances[1])
            {
                std::swap(distances[0], distances[1]);
                std::swap(vertices[0], vertices[1]);
            }

            if (distances[0] > 0.0f && distances[1] > 0.0f && distances[2] > 0.0f)
            {
                filteredTriangles.emplace_back(ModelTriangle(
                    vertices[0],
                    vertices[1],
                    vertices[2],
                    triangle.normal,
                    triangle.material));
            }
            // If one vertex outside clipping plane
            else if (distances[1] > 0.0f && distances[2] > 0.0f)
            {
                const float v1prop = clippingPlane.getIntersection(vertices.at(0).position, vertices.at(1).position);
                const float v2prop = clippingPlane.getIntersection(vertices.at(0).position, vertices.at(2).position);

                const ModelPoint newV1Vertex = Interpolation::interpolate(
                    vertices.at(0),
                    vertices.at(1),
                    v1prop);

                const ModelPoint newV2Vertex = Interpolation::interpolate(
                    vertices.at(0),
                    vertices.at(2),
                    v2prop);

                filteredTriangles.emplace_back(ModelTriangle(
                    newV1Vertex, vertices[1], vertices[2],
                    triangle.normal, triangle.material));

                filteredTriangles.emplace_back(ModelTriangle(
                    newV2Vertex, newV1Vertex, vertices[2],
                    triangle.normal, triangle.material));
            }
            // Check if two vertices outside plane
            else if (distances[2] > 0.0f)
            {
                const float v0prop = clippingPlane.getIntersection(vertices.at(0).position, vertices.at(2).position);
                const float v1prop = clippingPlane.getIntersection(vertices.at(1).position, vertices.at(2).position);

                filteredTriangles.emplace_back(ModelTriangle(
                    Interpolation::interpolate(vertices[0], vertices[2], v0prop),
                    Interpolation::interpolate(vertices[1], vertices[2], v1prop),
                    vertices[2],
                    triangle.normal,
                    triangle.material));
            }
        }
    }

    return filteredTriangles;
}

std::vector<ModelTriangle> Model::getPreparedTriangles(const Camera& camera) const
{
    return clipTriangles(toCameraSpace(camera, transformTriangles()));
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

    while (getline(MaterialFile, line))
    {
        const auto tokens = split(line, ' ');

        if (tokens.at(0) == "newmtl")
        {
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
