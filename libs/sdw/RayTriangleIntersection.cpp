#include "RayTriangleIntersection.h"

RayTriangleIntersection::RayTriangleIntersection() = default;
RayTriangleIntersection::RayTriangleIntersection(const ModelPoint& point, const glm::vec2& proportions, float distance,
                                                 const ModelTriangle& triangle, size_t index) :
		intersectionPoint(point),
		distanceFromCamera(distance),
		proportions(proportions),
		intersectedTriangle(triangle),
		triangleIndex(index) {}

std::ostream &operator<<(std::ostream &os, const RayTriangleIntersection &intersection) {
	os << "Intersection is at [" << intersection.intersectionPoint.position[0] << "," << intersection.intersectionPoint.position[1] << "," <<
	   intersection.intersectionPoint.position[2] << "] on triangle " << intersection.intersectedTriangle <<
	   " at a distance of " << intersection.distanceFromCamera;
	return os;
}
