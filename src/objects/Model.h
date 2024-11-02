#ifndef OBJECT_H
#define OBJECT_H
#include <../../libs/sdw/ModelTriangle.h>
#include <vector>

#include "Camera.h"
#include "Plane.h"
#include "../helper/MaterialMap.h"

class Model {
public:
    static constexpr float invSqrt2 = 0.70710678f;
    static std::vector<Plane> getClippingPlanes()
    {
        return{
            Plane(glm::vec3(0, 0, -1), -1), // Near plane
            Plane(glm::vec3(invSqrt2, 0, -invSqrt2), 0), // Left plane
            Plane(glm::vec3(-invSqrt2, 0, -invSqrt2), 0), // Right plane
            Plane(glm::vec3(0, -invSqrt2, -invSqrt2), 0), // Top plane
            Plane(glm::vec3(0, invSqrt2, -invSqrt2), 0), // Bottom plane
        };
    };

    static Model import(const char* objectPath);

    // Transforms all triangles into camera space
    static std::vector<ModelTriangle> transformTriangles(const Camera& camera, std::vector<ModelTriangle> triangles);

    // Transforms to camera space and clips to view
    static std::vector<ModelTriangle> clipTriangles(std::vector<ModelTriangle> triangles);

    // Get clipped and transformed triangles for raster render
    std::vector<ModelTriangle> getPreparedTriangles(const Camera& camera) const;

    MaterialMap materials;
    std::vector<ModelTriangle> triangles;

private:
    Model() = default;

    explicit Model(const std::vector<ModelTriangle> &triangles, const MaterialMap& materials);

    static MaterialMap importMaterials(const std::string &path);
};



#endif //OBJECT_H
