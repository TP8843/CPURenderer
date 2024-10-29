#ifndef RAYTRACER_H
#define RAYTRACER_H
#include <DrawingWindow.h>
#include <ModelTriangle.h>
#include <RayTriangleIntersection.h>

#include "../helper/Camera.h"
#include "../helper/Model.h"


class Raytracer
{
public:
    Raytracer(Model& model, Camera& camera);

    static std::pair<bool, RayTriangleIntersection> getClosestIntersection(glm::vec3 camera,
                                         glm::vec3 rayDirection,
                                         std::vector<ModelTriangle> triangles);

    void renderFrame(DrawingWindow &window);

    Model& model;
    Camera& camera;

private:
    static glm::vec3 getIntersectionWithTriangle(glm::vec3 camera, glm::vec3 rayDirection, ModelTriangle triangle);
};


#endif //RAYTRACER_H
