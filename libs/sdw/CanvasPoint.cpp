#include "CanvasPoint.h"

CanvasPoint::CanvasPoint() :
		texturePoint(-1, -1) {}

CanvasPoint::CanvasPoint(const float xPos, const float yPos) :
		x(xPos),
		y(yPos),
		brightness(1.0),
		texturePoint(-1, -1) {}

CanvasPoint::CanvasPoint(const float xPos, const float yPos, const float pointDepth) :
		x(xPos),
		y(yPos),
		depth(pointDepth),
		brightness(1.0),
		texturePoint(-1, -1) {}

CanvasPoint::CanvasPoint(const float xPos, const float yPos, const float pointDepth, const float pointBrightness) :
		x(xPos),
		y(yPos),
		depth(pointDepth),
		brightness(pointBrightness),
		texturePoint(-1, -1) {}

CanvasPoint::CanvasPoint(const float xPos, const float yPos, const TexturePoint texturePoint) :
		x(xPos),
		y(yPos),
		brightness(1.0),
		texturePoint(texturePoint)
{}

std::ostream &operator<<(std::ostream &os, const CanvasPoint &point) {
	os << "(" << point.x << ", " << point.y << ", " << point.depth << ") " << point.brightness;
	return os;
}
