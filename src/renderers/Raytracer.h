#ifndef RAYTRACER_H
#define RAYTRACER_H
#include <DrawingWindow.h>
#include <ModelTriangle.h>
#include <RayTriangleIntersection.h>

#include "../objects/Model.h"


class Raytracer
{
public:
    Raytracer(Model& model, Transformation& camera, Transformation& light);

    void renderFrame(DrawingWindow &window) const;

    Model& model;
    Transformation& camera;
    Transformation& light;

private:
     std::pair<Colour, int> fireRay(
        glm::vec3 startingPosition,
        glm::vec3 rayDirection,
        std::vector<ModelTriangle>& triangles,
        int previousIntersection = -1) const;

    static std::pair<bool, RayTriangleIntersection> getClosestIntersection(glm::vec3 camera,
                                                                           glm::vec3 rayDirection,
                                                                           std::vector<ModelTriangle>& triangles);

    static bool triangleIntersectsPoints(glm::vec3 point, glm::vec3 light, ModelTriangle triangle);

    static std::pair<bool, int> trianglesIntersectsPoints(glm::vec3 point, glm::vec3 light, int currentTriangleIndex, std::vector<ModelTriangle>& triangles);
};


#endif //RAYTRACER_H
