#pragma once

#include <cstdint>
#include <iostream>
#include <glm/gtc/integer.hpp>

struct Colour {
	std::string name;
	float red{};
	float green{};
	float blue{};
	Colour();
	Colour(float r, float g, float b);
	Colour(std::string n, float r, float g, float b);

	uint32_t asARGB() const
	{
		return (255 << 24) + (static_cast<int>(glm::round(red)) << 16)
			+ (static_cast<int>(glm::round(green)) << 8)
			+ static_cast<int>(glm::round(blue));
	};

	Colour operator+(const Colour &other) const;
	Colour operator-(const Colour &other) const;

	Colour operator*(const float num) const
	{
		return {name, glm::min(red * num, 255.f), glm::min(green * num, 255.f), glm::min(blue * num, 255.f)};
	}
};

std::ostream &operator<<(std::ostream &os, const Colour &colour);