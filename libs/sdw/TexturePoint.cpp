#include "TexturePoint.h"

TexturePoint::TexturePoint() = default;
TexturePoint::TexturePoint(float xPos, float yPos) : x(xPos), y(yPos) {}

TexturePoint TexturePoint::operator+(const TexturePoint &other) const
{
	return {x + other.x, y + other.y};
}

TexturePoint TexturePoint::operator-(const TexturePoint &other) const
{
	return {x - other.x, y - other.y};
}

std::ostream &operator<<(std::ostream &os, const TexturePoint &point) {
	os << "x: " << point.x << " y: " << point.y;
	return os;
}
