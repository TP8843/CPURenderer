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
        const ModelTriangle triangle = triangles[i];

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
            glm::vec3 position = rayDirection * possibleSolution.x;

            closestIntersection = std::pair<bool, RayTriangleIntersection>(true, RayTriangleIntersection(
                position,
                possibleSolution.x,
                triangle,
                i));
        }
    }

    return closestIntersection;
}

void Raytracer::renderFrame(DrawingWindow& window)
{
    for (float j = 0; j < static_cast<float>(window.height); j++)
    {
        for (float i = 0; i < static_cast<float>(window.width); i++)
        {

            glm::vec3 scenePosition = glm::normalize(glm::vec3(
                (i - static_cast<float>(window.width) / 2) / static_cast<float>(window.height),
                (j - static_cast<float>(window.height) / 2) / static_cast<float>(window.height),
                -camera.focalLength / 4
            ));

            std::pair<bool, RayTriangleIntersection> intersection =
                getClosestIntersection(camera.position, scenePosition, model.triangles);

            if (intersection.first)
            {
                window.setPixelColour(i, window.height - j, intersection.second.intersectedTriangle.material.colour.asARGB());
            }
        }
    }
}
