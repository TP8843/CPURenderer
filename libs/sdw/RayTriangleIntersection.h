#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include "ModelTriangle.h"

struct RayTriangleIntersection {
	ModelPoint intersectionPoint;
	float distanceFromCamera;
	ModelTriangle intersectedTriangle;
	size_t triangleIndex;
	glm::vec2 proportions;

	RayTriangleIntersection();
	RayTriangleIntersection(const ModelPoint& point, const glm::vec2& proportions, float distance,
	                        const ModelTriangle& triangle, size_t index);
	friend std::ostream &operator<<(std::ostream &os, const RayTriangleIntersection &intersection);
};
