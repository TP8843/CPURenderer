//
// Created by Thomas Parr on 29/10/2024.
//

#include "Raytracer.h"

#include <glm/glm.hpp>

#include <RayTriangleIntersection.h>

Raytracer::Raytracer(Model& model, Camera& camera) :
    model(model), camera(camera)
{}

std::pair<bool, RayTriangleIntersection> Raytracer::getClosestIntersection(glm::vec3 camera, glm::vec3 rayDirection,
                                                          std::vector<ModelTriangle> triangles)
{
    std::pair<bool, RayTriangleIntersection> closestIntersection =
        std::pair<bool, RayTriangleIntersection>(false, RayTriangleIntersection());

    for (int i = 0; i < triangles.size(); i++)
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
            (possibleSolution.x >=0) &&
            (possibleSolution.x < closestIntersection.second.distanceFromCamera || !closestIntersection.first))
        {
            closestIntersection.first = true;
            // glm::vec3 position = rayDirection * possibleSolution.x;

            glm::vec3 position = triangle.vertices[0]
                + (triangle.vertices[1] - triangle.vertices[0]) * possibleSolution.y
                + (triangle.vertices[2] - triangle.vertices[0]) * possibleSolution.z;

            closestIntersection = std::pair<bool, RayTriangleIntersection>(true, RayTriangleIntersection(
                position,
                glm::vec2(possibleSolution.y, possibleSolution.z),
                possibleSolution.x,
                triangle,
                i));
        }
    }

    return closestIntersection;
}

void Raytracer::renderFrame(DrawingWindow& window) const
{
    const float width = static_cast<float>(window.width);
    const float height = static_cast<float>(window.height);

    for (float j = 0; j < height; j++)
    {
        for (float i = 0; i < width; i++)
        {

            glm::vec3 scenePosition = glm::normalize(glm::vec3(
                (i - width / 2) / height,
                (j - height / 2) / height,
                -1
            ));

            std::pair<bool, RayTriangleIntersection> intersection =
                getClosestIntersection(camera.position, camera.rotation * scenePosition, model.triangles);

            if (intersection.first)
            {
                const auto lightPosition = glm::vec3(0,1,0);
                const auto lightDistance = glm::normalize(intersection.second.intersectionPoint - lightPosition);

                std::pair<bool, RayTriangleIntersection> closestToLight =
                    getClosestIntersection(lightPosition, glm::normalize(lightDistance), model.triangles);

                const bool inShadow = (closestToLight.first
                    && closestToLight.second.triangleIndex != intersection.second.triangleIndex);

                const float colourMultiplier = inShadow * 0.2 + !inShadow * 1;

                const Material& material = model.materials.getMaterial(intersection.second.intersectedTriangle.material);

                if (material.hasTexture())
                {
                    const auto texturePoints = intersection.second.intersectedTriangle.texturePoints;

                    const auto finalTexturePoint = texturePoints[0]
                        + (texturePoints[1] - texturePoints[0]) * intersection.second.proportions.x
                        + (texturePoints[2] - texturePoints[0]) * intersection.second.proportions.y;


                    // window.setPixelColour(i, height - j, material.getColour().asARGB());
                    window.setPixelColour(i, window.height - j, (material.getPixelTextureColour(finalTexturePoint.x, finalTexturePoint.y) * colourMultiplier).asARGB());
                }
                else
                {
                    window.setPixelColour(i, window.height - j, (material.getColour() * colourMultiplier).asARGB());
                }
            }
        }
    }
}
