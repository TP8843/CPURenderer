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

        glm::vec3 transformedNormal = normal;

        if (material.hasNormalMap())
        {
            const auto texturePoints = intersection.second.intersectedTriangle.texturePoints;
            const auto vertices = intersection.second.intersectedTriangle.vertices;

            const TexturePoint finalTexturePoint = texturePoints[0]
                + (texturePoints[1] - texturePoints[0]) * intersection.second.proportions.x
                + (texturePoints[2] - texturePoints[0]) * intersection.second.proportions.y;

            glm::vec3 edge0 = vertices.at(1) - vertices.at(0);
            glm::vec3 edge1 = vertices.at(2) - vertices.at(0);

            TexturePoint textureEdge0 = texturePoints.at(1) - texturePoints.at(0);
            TexturePoint textureEdge1 = texturePoints.at(2) - texturePoints.at(0);

            float r = 1.0f / (textureEdge0.x * textureEdge1.y - textureEdge0.y * textureEdge1.x);
            glm::vec3 tangent = glm::normalize((edge0 * textureEdge1.y - edge1 * textureEdge0.y) * r);
            glm::vec3 bitangent = glm::normalize((edge1 * textureEdge0.x - edge0 * textureEdge1.x) * r);

            glm::mat3 transformationMatrix = glm::mat3(
                tangent,
                bitangent,
                normal);

            transformedNormal = glm::normalize(transformationMatrix * material.getNormal(finalTexturePoint.x, finalTexturePoint.y));

            // return std::make_pair(glm::vec4(transformedNormal, 1.f), -1);
        }

        if (depth > 0 && material.getIlluminationModel() == MIRROR)
        {
            const auto result = mirror(
               rayDirection,
               intersection.second,
               triangles,
               transformedNormal,
               depth,
               previousShadowIntersection);

            return std::make_pair(result.first * material.getColour(),
                result.second);
        }

        if (depth > 0 && material.getIlluminationModel() == REFRACTION_NO_FRESNEL)
        {
            const auto result = refractIntoBox(
               rayDirection,
               intersection.second,
               triangles,
               transformedNormal,
               depth,
               previousShadowIntersection);

            return std::make_pair(result.first, result.second);
        }

        return surfaceColour(intersection.second, triangles, transformedNormal, material, previousShadowIntersection);
    }

    return std::make_pair(colour, previousShadowIntersection);
}

std::pair<glm::vec4, int> Raytracer::fireRayInsideBox(
    glm::vec3 startingPosition,
    glm::vec3 rayDirection,
    const std::vector<ModelTriangle>& triangles,
    const int depth,
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

        if (depth > 0)
        {
            const auto result = refractOutOfBox(
               rayDirection,
               intersection.second,
               triangles,
               intersection.second.intersectedTriangle.normal,
               depth,
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
    const float previousShadowIntersection) const
{
    const glm::vec3 reflectedRay = glm::normalize(rayDirection
        - 2.0f * (normal)
        * glm::dot(rayDirection, normal));

    return fireRay(
        intersection.intersectionPoint,
        reflectedRay,
        triangles,
        depth - 1,
        previousShadowIntersection);
}

std::pair<glm::vec4, int> Raytracer::refractIntoBox(
    const glm::vec3& rayDirection,
    const RayTriangleIntersection& intersection,
    const std::vector<ModelTriangle>& triangles,
    const glm::vec3& normal,
    int depth,
    float previousShadowIntersection) const
{
    const Material& material = scene.materials.getMaterial(intersection.intersectedTriangle.material);
    float currentRefractiveIndex = 1.f;
    float newRefractiveIndex = 1.33f;

    // std::cout << "Current refractive index: " << currentRefractiveIndex << ", New refractive index: " << newRefractiveIndex << std::endl;

    const float ratio = currentRefractiveIndex / newRefractiveIndex;

    const float cosThetaI = -glm::dot(rayDirection, normal);
    const float sin2ThetaT = glm::pow(ratio, 2.f) * (1.f - glm::pow(cosThetaI, 2.f));

    const glm::vec3 refractedRay = glm::normalize(ratio * rayDirection + (ratio * cosThetaI - glm::sqrt(1.f - sin2ThetaT)) * normal);

    return fireRayInsideBox(
        intersection.intersectionPoint,
        refractedRay,
        triangles,
        depth - 1,
        previousShadowIntersection);

}

std::pair<glm::vec4, int> Raytracer::refractOutOfBox(
    const glm::vec3& rayDirection,
    const RayTriangleIntersection& intersection,
    const std::vector<ModelTriangle>& triangles,
    const glm::vec3& normal,
    int depth,
    float previousShadowIntersection) const
{
    const Material& material = scene.materials.getMaterial(intersection.intersectedTriangle.material);
    float currentRefractiveIndex = 1.33f;
    float newRefractiveIndex = 1.f;

    // std::cout << "Current refractive index: " << currentRefractiveIndex << ", New refractive index: " << newRefractiveIndex << std::endl;

    const float ratio = currentRefractiveIndex / newRefractiveIndex;
    const float cosThetaI = glm::dot(rayDirection, normal);
    const float sin2ThetaT = glm::pow(ratio, 2.f) * (1.f - glm::pow(cosThetaI, 2.f));

    if (sin2ThetaT > 1.f)
    {
         const glm::vec3 reflectedRay = glm::normalize(rayDirection
             - 2.0f * (-normal)
             * glm::dot(rayDirection, -normal));

         return fireRayInsideBox(
             intersection.intersectionPoint,
             reflectedRay,
             triangles,
             depth - 1,
             previousShadowIntersection);

        return std::make_pair(glm::vec4(0, 1, 0, 0), -1);
    }
    else
    {
        if (material.getIlluminationModel() != REFRACTION_NO_FRESNEL) return surfaceColour(intersection, triangles, normal, material, previousShadowIntersection);

        const glm::vec3 refractedRay = glm::normalize(ratio * glm::normalize(rayDirection) + (ratio * cosThetaI - glm::sqrt(1.f - sin2ThetaT)) * normal);

        return fireRay(
            intersection.intersectionPoint,
            refractedRay,
            triangles,
            depth - 1,
            previousShadowIntersection);
    }
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

    for (int i = 0; i < SHADOW_SAMPLES; ++i)
    {
        const auto randomOffset = glm::vec3(
            static_cast<float>(random()) / (static_cast<float>(RAND_MAX) * 4.f) - 0.5f,
            static_cast<float>(random()) / (static_cast<float>(RAND_MAX) * 4.f) - 0.5f,
        static_cast<float>(random()) / (static_cast<float>(RAND_MAX) * 4.f) - 0.5f);

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

    const auto texturePoints = intersection.intersectedTriangle.texturePoints;
    const auto vertices = intersection.intersectedTriangle.vertices;
    glm::vec3 transformedNormal = normal;

    if (material.hasTexture())
    {
        const TexturePoint finalTexturePoint = texturePoints[0]
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
