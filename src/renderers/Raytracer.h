#ifndef RAYTRACER_H
#define RAYTRACER_H
#include <DrawingWindow.h>
#include <ModelTriangle.h>
#include <RayTriangleIntersection.h>

#include "../objects/Camera.h"
#include "../objects/Model.h"


class Raytracer
{
public:
    Raytracer(Model& model, Camera& camera);

    static std::pair<bool, RayTriangleIntersection> getClosestIntersection(glm::vec3 camera,
                                         glm::vec3 rayDirection,
                                         std::vector<ModelTriangle> triangles);

    void renderFrame(DrawingWindow &window) const;

    Model& model;
    Camera& camera;

private:
    static bool triangleIntersectsPoints(glm::vec3 point, glm::vec3 light, ModelTriangle triangle);
};


#endif //RAYTRACER_H
