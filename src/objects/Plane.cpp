#include "Plane.h"

Plane::Plane(const glm::vec3& normal, const float distance) :
    normal(normal), distance(distance)
{}

float Plane::distanceRelativeToPlane(const glm::vec3& point) const
{
    return glm::dot(normal, point) - distance;
}

float Plane::getIntersection(const glm::vec3& p1, const glm::vec3& p2) const
{
    return glm::clamp((-distance - glm::dot(normal, p1)) / glm::dot(normal, p2 - p1), 0.0f, 1.0f);
}
