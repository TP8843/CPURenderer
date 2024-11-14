#include <utility>
#include <vector>
#include <fstream>
#include <../../libs/sdw/Utils.h>
#include <../../libs/sdw/ModelTriangle.h>

#include "Model.h"
#include "../helper/Draw.h"
#include "../helper/Interpolation.h"
#include "../helper/StringHelpers.h"
#include "./materials/MaterialMap.h"

Model::Model(const std::vector<ModelTriangle> &triangles, MaterialMap materials) : materials(std::move(materials)),
    triangles(triangles) {
}

Model Model::import(const char *objectPath, const float scale) {
    std::string text;
    std::ifstream ObjectFile(objectPath);

    auto pathString = std::string(objectPath);

    // const auto lastSlashPos = pathString.find_last_of("/\\");
    // std::string parentPath = (std::string::npos == lastSlashPos)
    //                              ? ""
    //                              : pathString.substr(0, lastSlashPos);

    const std::string parentPath = StringHelpers::getFolderPath(pathString);

    MaterialMap materialMap = MaterialMap();
    std::string currentMaterial = "Backup";
    bool loadedMaterialFile = false;

    std::vector<glm::vec3> vertices;

    // Stores the total of the adjacent face normals and the number of adjacent faces (vertex normal total, number of normals)
    std::vector<std::pair<glm::vec3, float> > vertexNormalTotals;

    // The vertices for each triangle
    std::vector<std::vector<int> > triangleVertexIndices;

    std::vector<glm::vec3> customVertexNormals;
    std::vector<glm::vec2> vertexTextures;
    std::vector<ModelTriangle> triangles;

    while (getline(ObjectFile, text)) {
        text = StringHelpers::trimLine(text);

        // Split on all spaces
        const auto tokens = split(text, ' ');

        // Import materials
        if (tokens.at(0) == "mtllib") {
            materialMap = MaterialMap::import(materialMap, parentPath, tokens.at(1));
            loadedMaterialFile = true;
        }

        // Vertex
        if (tokens.at(0) == "v") {
            const auto v = glm::vec3(
                scale * std::stof(tokens.at(1)),
                scale * std::stof(tokens.at(2)),
                scale * std::stof(tokens.at(3)));

            vertices.emplace_back(v);

            vertexNormalTotals.emplace_back(glm::vec3(0, 0, 0), 0);
        }

        // Texture Points
        if (tokens.at(0) == "vt") {
            const auto vt = glm::vec2(
                std::stof(tokens.at(1)),
                std::stof(tokens.at(2)));

            vertexTextures.emplace_back(vt);
        }

        // Custom vector normal
        if (tokens.at(0) == "vn" && tokens.size() >= 4) {
            customVertexNormals.emplace_back(
                std::stof(tokens.at(1)),
                std::stof(tokens.at(2)),
                std::stof(tokens.at(3)));

        }

        // Material
        if (tokens.at(0) == "usemtl") {
            currentMaterial = tokens.at(1);
        }

        // Face
        if (tokens.at(0) == "f" && tokens.size() > 1 && !tokens.at(1).empty()) {
            std::vector<glm::vec3> faceVertices;
            std::vector<int> vertexIndices;
            std::vector<TexturePoint> polygonTexturePoints;
            std::vector<glm::vec3> polygonVertexNormals;
            bool hasTexture = false;
            bool hasNormal = false;

            for (int i = 1; i < tokens.size(); i++) {
                auto vertexTokens = split(tokens.at(i), '/');

                // Pull correct vertex from read in vertices
                faceVertices.push_back(vertices[std::stoi(vertexTokens.at(0)) - 1]);

                vertexIndices.push_back(std::stoi(vertexTokens.at(0)) - 1);

                // If vertex has material data
                if (vertexTokens.size() > 1 && !vertexTokens.at(1).empty()) {
                    hasTexture = true;
                    const auto point = vertexTextures.at(std::stoi(vertexTokens.at(1)) - 1);

                    polygonTexturePoints.emplace_back(
                        glm::mod(
                            point.x * static_cast<float>(materialMap.getMaterial(currentMaterial).getTextureWidth()),
                            static_cast<float>(materialMap.getMaterial(currentMaterial).getTextureWidth())),
                        glm::mod(
                            point.y * static_cast<float>(materialMap.getMaterial(currentMaterial).getTextureHeight()),
                            static_cast<float>(materialMap.getMaterial(currentMaterial).getTextureHeight())));
                }

                // If vertex has vertex normal data
                if (vertexTokens.size() > 2 && !vertexTokens.at(2).empty()) {
                    hasNormal = true;
                    const auto point = customVertexNormals.at(std::stoi(vertexTokens.at(2)) - 1);

                    polygonVertexNormals.push_back(point);
                }
            }

            // Add triangles for all parts of polygon
            for (int i = 1; i < faceVertices.size() - 1; i++) {
                triangleVertexIndices.push_back({vertexIndices.at(0), vertexIndices.at(i), vertexIndices.at(i + 1)});

                glm::vec3 normal;
                if (hasNormal) {
                    normal = glm::normalize(polygonVertexNormals.at(0) + polygonVertexNormals.at(i) + polygonVertexNormals.at(i + 1));
                } else {
                    // Calculate normal for triangle
                    normal = glm::normalize(glm::cross(
                        faceVertices.at(0) - faceVertices.at(i),
                        faceVertices.at(i + 1) - faceVertices.at(0)));

                    int vertex = vertexIndices.at(0);
                    auto vertexNormalTotal = vertexNormalTotals.at(vertex);
                    vertexNormalTotals.at(vertex) = std::pair<glm::vec3, float>(
                        vertexNormalTotal.first + normal,
                        vertexNormalTotal.second + 1);

                    vertex = vertexIndices.at(i);
                    vertexNormalTotal = vertexNormalTotals.at(vertex);
                    vertexNormalTotals.at(vertex) = std::pair<glm::vec3, float>(
                        vertexNormalTotal.first + normal,
                        vertexNormalTotal.second + 1);

                    vertex = vertexIndices.at(i + 1);
                    vertexNormalTotal = vertexNormalTotals.at(vertex);
                    vertexNormalTotals.at(vertex) = std::pair<glm::vec3, float>(
                        vertexNormalTotal.first + normal,
                        vertexNormalTotal.second + 1);
                }


                if (hasTexture && hasNormal) {
                    triangles.emplace_back(faceVertices[0], polygonTexturePoints[0], polygonVertexNormals[0],
                                           faceVertices[i], polygonTexturePoints[i], polygonVertexNormals[i],
                                           faceVertices[i + 1], polygonTexturePoints[i + 1], polygonVertexNormals[i + 1],
                                           normal,
                                           currentMaterial);
                } else if (hasNormal) {
                    triangles.emplace_back(faceVertices[0], polygonVertexNormals[0],
                                           faceVertices[i], polygonVertexNormals[i],
                                           faceVertices[i + 1], polygonVertexNormals[i + 1],
                                           normal,
                                           currentMaterial);
                } else if (hasTexture) {
                    triangles.emplace_back(faceVertices[0], polygonTexturePoints[0],
                                           faceVertices[i], polygonTexturePoints[i],
                                           faceVertices[i + 1], polygonTexturePoints[i + 1],
                                           normal,
                                           currentMaterial);
                } else {
                    triangles.emplace_back(faceVertices[0], faceVertices[i], faceVertices[i + 1], normal,
                                           currentMaterial);
                }
            }
        }
    }

    std::cout << "tranglevertex index" << triangleVertexIndices.size() << std::endl;

    // Add vertex normals to all triangles that don't have custom normals
    for (int i = 0; i < triangleVertexIndices.size(); i++)
    {
        if (!triangles[i].customNormals)
        {
            std::cout << "Reaching unreachable code :0 " << i << std::endl;
            const std::vector<int> &vertexIndices = triangleVertexIndices.at(i);

            for (int j = 0; j < vertexIndices.size(); j++)
            {
                const glm::vec3 vertexNormal = glm::normalize(
                    vertexNormalTotals.at(vertexIndices.at(j)).first / vertexNormalTotals.at(vertexIndices.at(j)).second);

                triangles[i].vertexNormals[j] = vertexNormal;
            }
        }
    }

    if (!loadedMaterialFile) {
        materialMap.addMaterial("Backup", Material(Colour(255, 255, 255), FLAT, 32.0f));
    }

    return Model(triangles, materialMap);
}

std::vector<ModelTriangle> Model::transformTriangles(const Camera &camera, std::vector<ModelTriangle> triangles) {
    auto newTriangles = std::vector<ModelTriangle>();
    const glm::mat3 normalRotation = camera.getNormalRotationMatrix();

    for (ModelTriangle &triangle: triangles) {
        newTriangles.emplace_back(
            (triangle.vertices[0] - camera.position) * camera.rotation,
            triangle.texturePoints[0],
            triangle.vertexNormals[0] * normalRotation,
            (triangle.vertices[1] - camera.position) * camera.rotation,
            triangle.texturePoints[1],
            triangle.vertexNormals[1] * normalRotation,
            (triangle.vertices[2] - camera.position) * camera.rotation,
            triangle.texturePoints[2],
            triangle.vertexNormals[2] * normalRotation,
            triangle.normal * normalRotation,
            triangle.material);
    }

    return newTriangles;
}

std::vector<ModelTriangle> Model::clipTriangles(std::vector<ModelTriangle> triangles) {
    auto filteredTriangles = std::vector<ModelTriangle>(std::move(triangles));

    for (const auto &clippingPlane: getClippingPlanes()) {
        std::vector<ModelTriangle> oldFilteredTriangles = filteredTriangles;
        filteredTriangles = std::vector<ModelTriangle>();

        for (const ModelTriangle &triangle: oldFilteredTriangles) {
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
            if (distances[0] > distances[1]) {
                std::swap(distances[0], distances[1]);
                std::swap(vertices[0], vertices[1]);
                std::swap(texturePoints[0], texturePoints[1]);
                std::swap(vertexNormals[0], vertexNormals[1]);
            }

            if (distances[1] > distances[2]) {
                std::swap(distances[1], distances[2]);
                std::swap(vertices[1], vertices[2]);
                std::swap(texturePoints[1], texturePoints[2]);
                std::swap(vertexNormals[1], vertexNormals[2]);
            }

            if (distances[0] > distances[1]) {
                std::swap(distances[0], distances[1]);
                std::swap(vertices[0], vertices[1]);
                std::swap(texturePoints[0], texturePoints[1]);
                std::swap(vertexNormals[0], vertexNormals[1]);
            }

            if (distances[0] > 0.0f && distances[1] > 0.0f && distances[2] > 0.0f) {
                filteredTriangles.emplace_back(
                    vertices[0], texturePoints[0], vertexNormals[0],
                    vertices[1], texturePoints[1], vertexNormals[1],
                    vertices[2], texturePoints[2], vertexNormals[2],
                    triangle.normal,
                    triangle.material);
            }
            // If one vertex outside clipping plane
            else if (distances[1] > 0.0f && distances[2] > 0.0f) {
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

                filteredTriangles.emplace_back(
                    newV1Vertex, newV1TexturePoint, newV1VertexNormal,
                    vertices[1], texturePoints[1], vertexNormals[1],
                    vertices[2], texturePoints[2], vertexNormals[2],
                    triangle.normal, triangle.material);

                filteredTriangles.emplace_back(
                    newV2Vertex, newV2TexturePoint, newV2VertexNormal,
                    newV1Vertex, newV1TexturePoint, newV1VertexNormal,
                    vertices[2], texturePoints[2], vertexNormals[2],
                    triangle.normal, triangle.material);
            }
            // Check if two vertices outside plane
            else if (distances[2] > 0.0f) {
                const float v0prop = clippingPlane.getIntersection(vertices[0], vertices[2]);
                const float v1prop = clippingPlane.getIntersection(vertices[1], vertices[2]);

                filteredTriangles.emplace_back(
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
                    triangle.material);
            }
        }
    }

    return filteredTriangles;
}

std::vector<ModelTriangle> Model::getPreparedTriangles(const Camera &camera) const {
    return clipTriangles(transformTriangles(camera, triangles));
}