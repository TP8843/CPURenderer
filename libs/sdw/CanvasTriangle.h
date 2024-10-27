#pragma once

#include "CanvasPoint.h"
#include <iostream>
#include <array>
#include <Colour.h>
#include <TextureMap.h>

struct CanvasTriangle {
	std::array<CanvasPoint, 3> vertices{};
	Colour colour;

	CanvasTriangle();
	CanvasTriangle(const CanvasPoint &v0, const CanvasPoint &v1, const CanvasPoint &v2, const Colour &c);
	CanvasPoint &v0();
	CanvasPoint &v1();
	CanvasPoint &v2();
	CanvasPoint operator[](size_t i) const;
	CanvasPoint &operator[](size_t i);
	friend std::ostream &operator<<(std::ostream &os, const CanvasTriangle &triangle);
};

std::ostream &operator<<(std::ostream &os, const CanvasTriangle &triangle);
