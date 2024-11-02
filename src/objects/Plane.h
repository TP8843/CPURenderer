#ifndef PLANE_H
#define PLANE_H

#include <glm/glm.hpp>

struct Plane {
    glm::vec3 normal;
    float distance;

    Plane(const glm::vec3 &normal, float distance);

    // distance > 0 when outside plane in direction of normal
    // distance < 0 when outside plane opposite direction of normal
    // distance == 0 when inside plane
    float distanceRelativeToPlane(const glm::vec3 &point) const;

    // Return proportion along line p1 to p2 of
    // intersection between line and plane
    // Only works if solution exists
    float getIntersection(const glm::vec3 &p1, const glm::vec3 &p2) const;
};



#endif //PLANE_H
