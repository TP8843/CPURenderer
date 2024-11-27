#include "Raytracer.h"
#include <glm/glm.hpp>
#include "../../libs/ctpl_stl.h"

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
    const std::vector<float> refractiveIndexes,
    int previousShadowIntersection) const
{
    auto colour = glm::vec4(0);

    std::pair<bool, RayTriangleIntersection> intersection =
        getClosestIntersection(startingPosition, rayDirection, triangles);

    if (intersection.first)
    {
        const std::array<glm::vec3, 3> vectorNormals = intersection.second.intersectedTriangle.vertexNormals;

        const Material& material = scene.materials.getMaterial(intersection.second.intersectedTriangle.material);

        const glm::vec3 normal = vectorNormals[0]
            + (vectorNormals[1] - vectorNormals[0]) * intersection.second.proportions.x
            + (vectorNormals[2] - vectorNormals[0]) * intersection.second.proportions.y;

        if (depth > 0 && material.getIlluminationModel() == MIRROR)
        {
            const auto result = mirror(
               rayDirection,
               intersection.second,
               triangles,
               normal,
               depth,
               refractiveIndexes,
               previousShadowIntersection);

            return std::make_pair(result.first * material.getColour(),
                result.second);
        }

        if (depth > 0 && material.getIlluminationModel() == REFRACTION_NO_FRESNEL)
        {
            const auto result = refract(
               rayDirection,
               intersection.second,
               triangles,
               normal,
               depth,
               refractiveIndexes,
               previousShadowIntersection);

            return std::make_pair(result.first, result.second);
        }

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
    const std::vector<float> refractiveIndexes,
    const float previousShadowIntersection) const
{
    const glm::vec3 reflectedRay = rayDirection
        - 2.0f * (normal)
        * glm::dot(rayDirection, normal);

    return fireRay(
        intersection.intersectionPoint,
        reflectedRay,
        triangles,
        depth - 1,
        refractiveIndexes,
        previousShadowIntersection);
}

std::pair<glm::vec4, int> Raytracer::refract(
    const glm::vec3& rayDirection,
    const RayTriangleIntersection& intersection,
    const std::vector<ModelTriangle>& triangles,
    const glm::vec3& normal,
    int depth,
    std::vector<float> refractiveIndexes,
    float previousShadowIntersection) const
{
    const Material& material = scene.materials.getMaterial(intersection.intersectedTriangle.material);
    const float currentRefractiveIndex = refractiveIndexes.back();
    float newRefractiveIndex;

    if (glm::dot(rayDirection, normal) > 0.f)
    {
        newRefractiveIndex = 1.5f;
    }
    else
    {
        refractiveIndexes.pop_back();
        newRefractiveIndex = refractiveIndexes.back();
    }

    const glm::vec3 perpendicular = rayDirection - (normal)
    * glm::dot(rayDirection, normal);

    const float ratio = newRefractiveIndex / currentRefractiveIndex;

    const glm::vec3 refractedRay = rayDirection - perpendicular - ratio * perpendicular;

    return fireRay(
        intersection.intersectionPoint,
        refractedRay,
        triangles,
        depth - 1,
        refractiveIndexes,
        previousShadowIntersection);
}

std::pair<glm::vec4, int> Raytracer::surfaceColour(
    const RayTriangleIntersection& intersection,
    const std::vector<ModelTriangle>& triangles,
    const glm::vec3& normal,
    const Material& material,
    int previousShadowIntersection) const
{
    glm::vec4 colour;
    float shadowProportion = 0.f;

    // const glm::vec3 lightDirection = glm::normalize(scene.light.position - intersection.intersectionPoint);
    // const glm::vec3 lightTangent = glm::normalize(-glm::cross(lightDirection, glm::vec3(0.f, 1.f, 0.f)));
    // const glm::vec3 lightBitangent = glm::normalize(glm::cross(lightTangent, lightDirection));

    for (int i = 0; i < SHADOW_SAMPLES; ++i)
    {
        const auto randomOffset = glm::vec3(
            static_cast<float>(random()) / (static_cast<float>(RAND_MAX) * 4.f) - 0.5f,
            static_cast<float>(random()) / (static_cast<float>(RAND_MAX) * 4.f) - 0.5f,
        static_cast<float>(random()) / (static_cast<float>(RAND_MAX) * 4.f) - 0.5f);

        // const auto pointRadius = randomOffset.x / 1.5f;
        // const auto pointAngle = randomOffset.y * 2 * M_PI;
        // const auto discPos = glm::vec2(pointRadius * glm::cos(pointAngle), pointRadius * glm::sin(pointAngle));

        const auto finalLightPosition = scene.light.position + randomOffset;

        if (previousShadowIntersection != -1)
        {
            if (triangleIntersectsPoints(
                intersection.intersectionPoint,
                finalLightPosition,
                triangles.at(previousShadowIntersection)))
            {
                shadowProportion += 1.f;
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
                    finalLightPosition,
                    intersection.triangleIndex,
                    triangles);

            if (lightIntersection.first)
            {
                shadowProportion += 1.f;
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
            shadowProportion / SHADOW_SAMPLES);
    }
    else
    {
        // const glm::vec4 v0Colour = material.getColourAtPointInCameraSpace(
        //     scene.camera,
        //     scene.light,
        //     (intersection.intersectedTriangle.vertices[0] - scene.camera.position) * scene.camera.rotation,
        //     intersection.intersectedTriangle.vertexNormals[0] * scene.camera.getNormalRotationMatrix(),
        //     material.getIlluminationModel(),
        //     shadowProportion / SHADOW_SAMPLES);
        //
        // const glm::vec4 v1Colour = material.getColourAtPointInCameraSpace(
        //     scene.camera,
        //     scene.light,
        //     (intersection.intersectedTriangle.vertices[1] - scene.camera.position) * scene.camera.rotation,
        //     intersection.intersectedTriangle.vertexNormals[1] * scene.camera.getNormalRotationMatrix(),
        //     material.getIlluminationModel(),
        //     shadowProportion / SHADOW_SAMPLES);
        //
        // const glm::vec4 v2Colour = material.getColourAtPointInCameraSpace(
        //     scene.camera,
        //     scene.light,
        //     (intersection.intersectedTriangle.vertices[2] - scene.camera.position) * scene.camera.rotation,
        //     intersection.intersectedTriangle.vertexNormals[2] * scene.camera.getNormalRotationMatrix(),
        //     material.getIlluminationModel(),
        //     shadowProportion / SHADOW_SAMPLES);
        //
        // colour = v0Colour + v1Colour * intersection.proportions.x + v2Colour * intersection.proportions.y;

        colour = material.getColourAtPointInCameraSpace(
            scene.camera,
            scene.light,
            (intersection.intersectionPoint - scene.camera.position) * scene.camera.rotation,
            normal * scene.camera.getNormalRotationMatrix(),
            material.getIlluminationModel(),
            shadowProportion / SHADOW_SAMPLES);
    }

    return std::make_pair(colour, previousShadowIntersection);
}

void Raytracer::renderRow(
    DrawingWindow& window,
    const std::vector<ModelTriangle>& triangles,
    const float width,
    const float height,
    const int row) const
{
    int previousLightIntersection = -1;
    for (size_t i = 0; i < window.width; i++)
    {
        glm::vec3 scenePosition = glm::normalize(glm::vec3(
            (static_cast<float>(i) - width / 2) / height,
            (static_cast<float>(row) + 1 - height / 2) / height,
            -scene.camera.scale
        ));

        const std::pair<glm::vec4, int> final = fireRay(
            scene.camera.position,
            scene.camera.rotation * scenePosition,
            triangles,
            10,
            std::vector<float>(1.f),
            previousLightIntersection);

        previousLightIntersection = final.second;

        window.setPixelColour(i, window.height - row - 1, Material::getScreenColour(final.first));
    }
}

void Raytracer::renderFrame(DrawingWindow& window) const
{
    ctpl::thread_pool pool(std::thread::hardware_concurrency());
    auto jobs = std::vector<std::future<void>>();
    const auto width = static_cast<float>(window.width);
    const auto height = static_cast<float>(window.height);

    auto transformedTriangles = scene.getTransformedTriangles();

    for (size_t j = 0; j < window.height; j++)
    {
        jobs.push_back(pool.push([this, transformedTriangles, width, height, j, &window](int id) -> void
        {
            renderRow(window, transformedTriangles, width, height, j);
        }));
    }

    for (auto& job : jobs)
    {
        job.get();
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

bool Raytracer::triangleIntersectsPoints(const glm::vec3 point, const glm::vec3 light, const ModelTriangle& triangle)
{
    const glm::vec3 ray = light - point;
    const float rayDistance = glm::length(ray);

    const glm::vec3 e0 = triangle.vertices[1] - triangle.vertices[0];
    const glm::vec3 e1 = triangle.vertices[2] - triangle.vertices[0];
    const glm::vec3 SPVector = point - triangle.vertices[0];
    const glm::mat3 DEMatrix(-glm::normalize(ray), e0, e1);
    const glm::vec3 possibleSolution = glm::inverse(DEMatrix) * SPVector;

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
