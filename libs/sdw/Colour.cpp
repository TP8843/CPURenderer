#include "Colour.h"
#include <glm/glm.hpp>

Colour::Colour() = default;
Colour::Colour(float r, float g, float b) : red(r), green(g), blue(b) {}
Colour::Colour(std::string n, float r, float g, float b) :
		name(std::move(n)),
		red(r), green(g), blue(b) {}

std::ostream &operator<<(std::ostream &os, const Colour &colour) {
	os << colour.name << " ["
	   << colour.red << ", "
	   << colour.green << ", "
	   << colour.blue << "]";
	return os;
}

Colour Colour::operator+(const Colour& other) const
{
	return {
		name,
		glm::min(red + other.red, 255.f),
		glm::min(green + other.green, 255.f),
		glm::min(blue + other.blue, 255.f)
	};
}

Colour Colour::operator-(const Colour& other) const
{
	return {
		name,
		glm::max(red - other.red, 0.f),
		glm::max(green - other.green, 0.f),
		glm::max(blue - other.blue, 0.f)
	};
}
