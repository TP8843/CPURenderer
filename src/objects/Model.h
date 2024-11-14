#ifndef OBJECT_H
#define OBJECT_H
#include <../../libs/sdw/ModelTriangle.h>
#include <vector>

#include "Camera.h"
#include "Plane.h"
#include "Transformation.h"
#include "./materials/MaterialMap.h"

class Model
{
public:
    static constexpr float invSqrt2 = 0.70710678f;

    static std::vector<Plane> getClippingPlanes()
    {
        return {
            Plane(glm::vec3(0, 0, -1), 1), // Near plane
            Plane(glm::vec3(invSqrt2, 0, -invSqrt2), 0), // Left plane
            Plane(glm::vec3(-invSqrt2, 0, -invSqrt2), 0), // Right plane
            Plane(glm::vec3(0, -invSqrt2, -invSqrt2), 0), // Top plane
            Plane(glm::vec3(0, invSqrt2, -invSqrt2), 0), // Bottom plane
        };
    }

    static Model import(const char* objectPath, Transformation& transformation);

    // Transforms all triangles via the Transformation property
    std::vector<ModelTriangle> getTransformedTriangles() const;

    // Transforms all triangles into camera space
    static std::vector<ModelTriangle> mapToCameraSpace(const Camera& camera, std::vector<ModelTriangle> triangles);

    // Transforms to camera space and clips to view
    static std::vector<ModelTriangle> clipTriangles(std::vector<ModelTriangle> triangles);

    // Get clipped and transformed triangles for raster render
    std::vector<ModelTriangle> getRasterPreparedTriangles(const Camera& camera) const;

    MaterialMap materials;
    std::vector<ModelTriangle> triangles;

    Transformation& transformation;

    float scale = 1.0;
    glm::vec3 translation = glm::vec3(0, 0, 0);
    glm::mat3 rotation = glm::mat3(1);

private:
    explicit Model(const std::vector<ModelTriangle>& triangles,
                   MaterialMap materials,
                   Transformation& transformation);
};


#endif //OBJECT_H
