#include "Raytracer.h"
#include <glm/glm.hpp>

#include <RayTriangleIntersection.h>

Raytracer::Raytracer(Model& model, Camera& camera, Light& light) :
    model(model), camera(camera), light(light)
{
}

std::pair<bool, RayTriangleIntersection> Raytracer::getClosestIntersection(glm::vec3 camera,
                                                                           glm::vec3 rayDirection,
                                                                           std::vector<ModelTriangle> triangles)
{
    auto closestIntersection =
        std::pair<bool, RayTriangleIntersection>(false, RayTriangleIntersection());

    for (size_t i = 0; i < triangles.size(); i++)
    {
        const ModelTriangle& triangle = triangles[i];

        glm::vec3 e0 = triangle.vertices.at(1).position - triangle.vertices.at(0).position;
        glm::vec3 e1 = triangle.vertices.at(2).position - triangle.vertices.at(0).position;
        glm::vec3 SPVector = camera - triangle.vertices.at(0).position;
        glm::mat3 DEMatrix(-rayDirection, e0, e1);
        glm::vec3 possibleSolution = glm::inverse(DEMatrix) * SPVector;

        if ((possibleSolution.y >= 0.0 && possibleSolution.y <= 1.0) &&
            (possibleSolution.z >= 0.0 && possibleSolution.z <= 1.0) &&
            (possibleSolution.y + possibleSolution.z <= 1.0) &&
            (possibleSolution.x >= 0) &&
            (possibleSolution.x < closestIntersection.second.distanceFromCamera || !closestIntersection.first))
        {
            closestIntersection.first = true;
            // glm::vec3 position = rayDirection * possibleSolution.x;

            ModelPoint position = triangle.vertices.at(0)
                + (triangle.vertices.at(1) - triangle.vertices.at(0)) * possibleSolution.y
                + (triangle.vertices.at(2) - triangle.vertices.at(0)) * possibleSolution.z;

            closestIntersection = std::pair<bool, RayTriangleIntersection>(true, RayTriangleIntersection(
                                                                               position,
                                                                               glm::vec2(possibleSolution.y,
                                                                                   possibleSolution.z),
                                                                               possibleSolution.x,
                                                                               triangle,
                                                                               i));
        }
    }

    return closestIntersection;
}

void Raytracer::renderFrame(DrawingWindow& window) const
{
    const auto width = static_cast<float>(window.width);
    const auto height = static_cast<float>(window.height);

    const std::vector<ModelTriangle> scaledTriangles = model.transformTriangles();

    bool hasPreviousLightIntersection = false;
    ModelTriangle previousLightIntersection;

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
                getClosestIntersection(camera.position, camera.rotation * scenePosition, scaledTriangles);

            if (intersection.first)
            {
                const auto lightDisplacement = intersection.second.intersectionPoint.position - light.position;

                bool inShadow = false;

                if (hasPreviousLightIntersection)
                {
                    if (triangleIntersectsPoints(
                        intersection.second.intersectionPoint.position,
                        light.position,
                        previousLightIntersection))
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
                    std::pair<bool, RayTriangleIntersection> closestToLight = getClosestIntersection(
                        light.position,
                        glm::normalize(lightDisplacement),
                        scaledTriangles);

                    inShadow = closestToLight.first
                        && closestToLight.second.distanceFromCamera - glm::length(lightDisplacement) < 0.00000002f
                        && closestToLight.second.triangleIndex != intersection.second.triangleIndex;

                    if (inShadow)
                    {
                        hasPreviousLightIntersection = true;
                        previousLightIntersection = closestToLight.second.intersectedTriangle;
                    }
                }

                const float colourMultiplier = inShadow ? 0.2f
                       // Point must be in camera space for specular highlight calculations
                       : light.getMultiplier(camera,
                                             (intersection.second.intersectionPoint.position - camera.position)
                                                * camera.rotation,
                                             intersection.second.intersectionPoint.normal);

                const Material& material = model.materials.
                                                 getMaterial(intersection.second.intersectedTriangle.material);

                if (material.hasTexture())
                {
                    const auto finalTexturePoint = intersection.second.intersectionPoint.texturePoint;

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

    glm::vec3 e0 = triangle.vertices.at(1).position - triangle.vertices.at(0).position;
    glm::vec3 e1 = triangle.vertices.at(2).position - triangle.vertices.at(0).position;
    glm::vec3 SPVector = point - triangle.vertices.at(0).position;
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
