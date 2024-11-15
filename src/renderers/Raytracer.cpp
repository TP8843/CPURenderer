#include "Raytracer.h"
#include <glm/glm.hpp>

#include <RayTriangleIntersection.h>

Raytracer::Raytracer(Model& model, Transformation& camera, Transformation& light) :
    model(model), camera(camera), light(light)
{
}

std::pair<bool, RayTriangleIntersection> Raytracer::getClosestIntersection(glm::vec3 camera,
                                                                           glm::vec3 rayDirection,
                                                                           std::vector<ModelTriangle>& triangles)
{
    auto closestIntersection =
        std::pair<bool, RayTriangleIntersection>(false, RayTriangleIntersection());

    bool hasIntersection = false;
    glm::vec3 closestSolution;
    int closestIndex = -1;

    for (size_t i = 0; i < triangles.size(); i++)
    {
        const ModelTriangle& triangle = triangles[i];

        glm::vec3 e0 = triangle.vertices[1] - triangle.vertices[0];
        glm::vec3 e1 = triangle.vertices[2] - triangle.vertices[0];
        glm::vec3 SPVector = camera - triangle.vertices[0];
        glm::mat3 DEMatrix(-rayDirection, e0, e1);
        glm::vec3 possibleSolution = glm::inverse(DEMatrix) * SPVector;

        if ((possibleSolution.y >= 0.0 && possibleSolution.y <= 1.0) &&
            (possibleSolution.z >= 0.0 && possibleSolution.z <= 1.0) &&
            (possibleSolution.y + possibleSolution.z <= 1.0) &&
            (possibleSolution.x >= 0) &&
            (possibleSolution.x < closestSolution.x || !hasIntersection))
        {
            hasIntersection = true;
            closestSolution = possibleSolution;
            closestIndex = i;
        }
    }

    if (hasIntersection)
    {
        ModelTriangle& closestTriangle = triangles.at(closestIndex);
        glm::vec3 position = closestTriangle.vertices[0]
        + (closestTriangle.vertices[1] - closestTriangle.vertices[0]) * closestSolution.y
        + (closestTriangle.vertices[2] - closestTriangle.vertices[0]) * closestSolution.z;

        return std::pair<bool, RayTriangleIntersection>(true, RayTriangleIntersection(
            position,
            glm::vec2(closestSolution.y, closestSolution.z),
            closestSolution.x,
            closestTriangle,
            closestIndex));
    }

    return std::pair<bool, RayTriangleIntersection>(false, RayTriangleIntersection());
}

void Raytracer::renderFrame(DrawingWindow& window) const
{
    const auto width = static_cast<float>(window.width);
    const auto height = static_cast<float>(window.height);

    bool hasPreviousLightIntersection = false;
    int previousLightIntersection;

    auto transformedTriangles = model.getTransformedTriangles();

    for (size_t j = 0; j < window.height; j++)
    {
        for (size_t i = 0; i < window.width; i++)
        {
            glm::vec3 scenePosition = glm::normalize(glm::vec3(
                (static_cast<float>(i) - width / 2) / height,
                (static_cast<float>(j) + 1 - height / 2) / height,
                -1
            ));

            std::pair<bool, RayTriangleIntersection> intersection =
                getClosestIntersection(camera.position, camera.rotation * scenePosition, transformedTriangles);

            if (intersection.first)
            {
                bool inShadow = false;

                if (hasPreviousLightIntersection)
                {
                    if (triangleIntersectsPoints(
                        intersection.second.intersectionPoint,
                        light.position,
                        transformedTriangles.at(previousLightIntersection)))
                    {
                        inShadow = true;
                    }
                    else
                    {
                        hasPreviousLightIntersection = false;
                    }
                }

                if (!hasPreviousLightIntersection)
                {
                    auto lightIntersection=
                        trianglesIntersectsPoints(intersection.second.intersectionPoint, light.position, intersection.second.triangleIndex, transformedTriangles);

                    inShadow = lightIntersection.first;

                    if (inShadow)
                    {
                        hasPreviousLightIntersection = true;
                        previousLightIntersection = lightIntersection.second;
                    }
                }

                const std::array<glm::vec3, 3> vectorNormals = intersection.second.intersectedTriangle.vertexNormals;

                const glm::vec3 normal = vectorNormals[0]
                        + (vectorNormals[1] - vectorNormals[0]) * intersection.second.proportions.x
                        + (vectorNormals[2] - vectorNormals[0]) * intersection.second.proportions.y;

                const Material& material = model.materials.
                                                 getMaterial(intersection.second.intersectedTriangle.material);

                const float colourMultiplier = inShadow ? 0.2f
                    // Point must be in camera space for specular highlight calculations
                    : material.getColourAtPointInCameraSpace(
                        camera,
                        light,
                        (intersection.second.intersectionPoint - camera.position) * camera.rotation,
                        normal * camera.getNormalRotationMatrix());

                if (material.hasTexture())
                {
                    const auto texturePoints = intersection.second.intersectedTriangle.texturePoints;

                    const auto finalTexturePoint = texturePoints[0]
                        + (texturePoints[1] - texturePoints[0]) * intersection.second.proportions.x
                        + (texturePoints[2] - texturePoints[0]) * intersection.second.proportions.y;

                    window.setPixelColour(i, window.height - j - 1,
                                          (material.getPixelTextureColour(
                                               static_cast<int>(finalTexturePoint.x),
                                               static_cast<int>(finalTexturePoint.y)) * colourMultiplier).asARGB());
                }
                else
                {
                    window.setPixelColour(i, window.height - j - 1, (material.getColour() * colourMultiplier).asARGB());
                }
            }
        }
    }
}

bool Raytracer::triangleIntersectsPoints(glm::vec3 point, glm::vec3 light, ModelTriangle triangle)
{
    glm::vec3 ray = light - point;
    float rayDistance = glm::length(ray);

    glm::vec3 e0 = triangle.vertices[1] - triangle.vertices[0];
    glm::vec3 e1 = triangle.vertices[2] - triangle.vertices[0];
    glm::vec3 SPVector = point - triangle.vertices[0];
    glm::mat3 DEMatrix(-glm::normalize(ray), e0, e1);
    glm::vec3 possibleSolution = glm::inverse(DEMatrix) * SPVector;

    if ((possibleSolution.y >= 0.0 && possibleSolution.y <= 1.0) &&
        (possibleSolution.z >= 0.0 && possibleSolution.z <= 1.0) &&
        (possibleSolution.y + possibleSolution.z <= 1.0) &&
        (possibleSolution.x >= 0.01) &&
        rayDistance > possibleSolution.x)
    {
        return true;
    }

    return false;
}

std::pair<bool, int> Raytracer::trianglesIntersectsPoints(glm::vec3 point, glm::vec3 light,
    const int currentTriangleIndex,
    std::vector<ModelTriangle>& triangles)
{
    glm::vec3 ray = light - point;
    float rayDistance = glm::length(ray);

    for (int i = 0; i < triangles.size(); i++)
    {
        ModelTriangle& triangle = triangles.at(i);

        glm::vec3 e0 = triangle.vertices[1] - triangle.vertices[0];
        glm::vec3 e1 = triangle.vertices[2] - triangle.vertices[0];
        glm::vec3 SPVector = point - triangle.vertices[0];
        glm::mat3 DEMatrix(-glm::normalize(ray), e0, e1);
        glm::vec3 possibleSolution = glm::inverse(DEMatrix) * SPVector;

        if ((possibleSolution.y >= 0.0 && possibleSolution.y <= 1.0) &&
            (possibleSolution.z >= 0.0 && possibleSolution.z <= 1.0) &&
            (possibleSolution.y + possibleSolution.z <= 1.0) &&
            (possibleSolution.x >= 0.01) &&
            rayDistance > possibleSolution.x &&
            currentTriangleIndex != i)
        {
            return std::make_pair(true, i);
        }
    }

    return std::make_pair(false, -1);
}