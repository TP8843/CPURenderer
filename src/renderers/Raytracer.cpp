#include "Raytracer.h"
#include <glm/glm.hpp>

#include <RayTriangleIntersection.h>

Raytracer::Raytracer(Scene& scene) :
    scene(scene)
{
}

std::pair<glm::vec4, int> Raytracer::fireRay(
    glm::vec3 startingPosition,
    glm::vec3 rayDirection,
    const std::vector<ModelTriangle>& triangles,
    const int depth,
    int previousShadowIntersection) const
{
    auto colour = glm::vec4(0);

    if (depth == 0) return std::make_pair(colour, -1);

    std::pair<bool, RayTriangleIntersection> intersection =
        getClosestIntersection(startingPosition, rayDirection, triangles);

    if (intersection.first)
    {
        const std::array<glm::vec3, 3> vectorNormals = intersection.second.intersectedTriangle.vertexNormals;

        const glm::vec3 normal = vectorNormals[0]
            + (vectorNormals[1] - vectorNormals[0]) * intersection.second.proportions.x
            + (vectorNormals[2] - vectorNormals[0]) * intersection.second.proportions.y;

        const Material& material = scene.materials.
                                         getMaterial(intersection.second.intersectedTriangle.material);

        if (material.getIlluminationModel() == MIRROR)
            return mirror(rayDirection, intersection.second, triangles, normal, depth, previousShadowIntersection);

        return surfaceColour(intersection.second, triangles, normal, material, previousShadowIntersection);
    }

    return std::make_pair(colour, previousShadowIntersection);
}

std::pair<glm::vec4, int> Raytracer::mirror(
    const glm::vec3& rayDirection,
    const RayTriangleIntersection& intersection,
    const std::vector<ModelTriangle>& triangles,
    const glm::vec3& normal,
    const int depth,
    const float previousShadowIntersection) const
{
    const glm::vec3 reflectedRay = rayDirection
        - 2.0f * normal * glm::dot(rayDirection, normal);

    return fireRay(intersection.intersectionPoint, reflectedRay, triangles, depth - 1, previousShadowIntersection);
}

std::pair<glm::vec4, int> Raytracer::surfaceColour(
    const RayTriangleIntersection& intersection,
    const std::vector<ModelTriangle>& triangles,
    const glm::vec3& normal,
    const Material& material,
    int previousShadowIntersection) const
{
    glm::vec4 colour;
    float shadowProportion = false;
    constexpr float shadowCastCount = 200.f;

    for (int i = 0; i < shadowCastCount; ++i)
    {

        const auto randomOffset = glm::vec3(
            static_cast<float>(random()) / (static_cast<float>(RAND_MAX) * 5.f) - 0.5f,
            static_cast<float>(random()) / (static_cast<float>(RAND_MAX) * 5.f) - 0.5f,
            static_cast<float>(random()) / (static_cast<float>(RAND_MAX) * 5.f) - 0.5f);

        if (previousShadowIntersection != -1)
        {
            if (triangleIntersectsPoints(
                intersection.intersectionPoint,
                scene.light.position + randomOffset,
                triangles.at(previousShadowIntersection)))
            {
                shadowProportion += 1.f / shadowCastCount;
            }
            else
            {
                previousShadowIntersection = -1;
            }
        }

        if (previousShadowIntersection == -1)
        {
            const auto lightIntersection =
                trianglesIntersectsPoints(
                    intersection.intersectionPoint,
                    scene.light.position + randomOffset,
                    intersection.triangleIndex,
                    triangles);

            shadowProportion += lightIntersection.first? 1.f / shadowCastCount : 0.f;

            if (shadowProportion)
            {
                previousShadowIntersection = lightIntersection.second;
            }
        }
    }

    if (material.hasTexture())
    {
        const auto texturePoints = intersection.intersectedTriangle.texturePoints;

        const auto finalTexturePoint = texturePoints[0]
            + (texturePoints[1] - texturePoints[0]) * intersection.proportions.x
            + (texturePoints[2] - texturePoints[0]) * intersection.proportions.y;

        colour = material.getColourAtPointInCameraSpace(
            scene.camera,
            scene.light,
            (intersection.intersectionPoint - scene.camera.position) * scene.camera.rotation,
            normal * scene.camera.getNormalRotationMatrix(),
            finalTexturePoint,
            material.getIlluminationModel(),
            shadowProportion);
    }
    else
    {
        colour = material.getColourAtPointInCameraSpace(
            scene.camera,
            scene.light,
            (intersection.intersectionPoint - scene.camera.position) * scene.camera.rotation,
            normal * scene.camera.getNormalRotationMatrix(),
            material.getIlluminationModel(),
            shadowProportion);
    }

    return std::make_pair(colour, previousShadowIntersection);
}

void Raytracer::renderFrame(DrawingWindow& window) const
{
    const auto width = static_cast<float>(window.width);
    const auto height = static_cast<float>(window.height);

    int previousLightIntersection = -1;

    auto transformedTriangles = scene.getTransformedTriangles();

    for (size_t j = 0; j < window.height; j++)
    {
        for (size_t i = 0; i < window.width; i++)
        {
            glm::vec3 scenePosition = glm::normalize(glm::vec3(
                (static_cast<float>(i) - width / 2) / height,
                (static_cast<float>(j) + 1 - height / 2) / height,
                -1
            ));

            const std::pair<glm::vec4, int> final = fireRay(scene.camera.position, scene.camera.rotation * scenePosition,
                                                            transformedTriangles, 20, previousLightIntersection);

            previousLightIntersection = final.second;

            window.setPixelColour(i, window.height - j - 1, Material::getScreenColour(final.first));
        }
    }
}

std::pair<bool, RayTriangleIntersection> Raytracer::getClosestIntersection(
    glm::vec3 camera,
    glm::vec3 rayDirection,
    const std::vector<ModelTriangle>& triangles)
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
            (possibleSolution.x >= 0.00001f) &&
            (possibleSolution.x < closestSolution.x || !hasIntersection))
        {
            hasIntersection = true;
            closestSolution = possibleSolution;
            closestIndex = i;
        }
    }

    if (hasIntersection)
    {
        const ModelTriangle& closestTriangle = triangles.at(closestIndex);
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

bool Raytracer::triangleIntersectsPoints(glm::vec3 point, glm::vec3 light, const ModelTriangle& triangle)
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
                                                          const std::vector<ModelTriangle>& triangles)
{
    glm::vec3 ray = light - point;
    float rayDistance = glm::length(ray);

    for (int i = 0; i < triangles.size(); i++)
    {
        const ModelTriangle& triangle = triangles.at(i);

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
            return std::pair<bool, int>(true, i);
        }
    }

    return std::pair<bool, int>(false, -1);
}
