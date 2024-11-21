#ifndef RAYTRACER_H
#define RAYTRACER_H
#include <DrawingWindow.h>
#include <ModelTriangle.h>
#include <RayTriangleIntersection.h>

#include "../objects/Model.h"
#include "../objects/Scene.h"


class Raytracer
{
public:
    Raytracer(Scene& scene);

    void renderFrame(DrawingWindow& window) const;

    Scene& scene;

private:
    std::pair<glm::vec4, int> fireRay(
        glm::vec3 startingPosition,
        glm::vec3 rayDirection,
        const std::vector<ModelTriangle>& triangles,
        int depth,
        int previousShadowIntersection = -1) const;

    std::pair<glm::vec4, int> mirror(
        const glm::vec3& rayDirection,
        const RayTriangleIntersection& intersection,
        const std::vector<ModelTriangle>& triangles,
        const glm::vec3& normal,
        int depth,
        float previousShadowIntersection) const;

    std::pair<glm::vec4, int> surfaceColour(
        const RayTriangleIntersection& intersection,
        const std::vector<ModelTriangle>& triangles,
        const glm::vec3& normal,
        const Material& material,
        int previousShadowIntersection) const;

    static std::pair<bool, RayTriangleIntersection> getClosestIntersection(
        glm::vec3 camera,
        glm::vec3 rayDirection,
        const std::vector<ModelTriangle>& triangles);

    static bool triangleIntersectsPoints(glm::vec3 point, glm::vec3 light, const ModelTriangle& triangle);

    static std::pair<bool, int> trianglesIntersectsPoints(
        glm::vec3 point,
        glm::vec3 light,
        int currentTriangleIndex,
        const std::vector<ModelTriangle>& triangles);
};


#endif //RAYTRACER_H
