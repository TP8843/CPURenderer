#include "ModelTriangle.h"

ModelTriangle::ModelTriangle() = default;

ModelTriangle::ModelTriangle(ModelPoint v0,
                             ModelPoint v1,
                             ModelPoint v2,
                             const glm::vec3& normal,
                             std::string material) :
    vertices({{v0, v1, v2}}), material(std::move(material)), normal(normal)
{
}

std::ostream& operator<<(std::ostream& os, const ModelTriangle& triangle)
{
    os << "(" << triangle.vertices[0].position.x << ", " << triangle.vertices[0].position.y << ", " << triangle.vertices[0].position.z << ")\n";
    os << "(" << triangle.vertices[1].position.x << ", " << triangle.vertices[1].position.y << ", " << triangle.vertices[1].position.z << ")\n";
    os << "(" << triangle.vertices[2].position.x << ", " << triangle.vertices[2].position.y << ", " << triangle.vertices[2].position.z << ")\n";
    return os;
}
