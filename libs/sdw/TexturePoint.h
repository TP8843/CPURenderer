#pragma once

#include <iostream>

struct TexturePoint {
	float x{};
	float y{};

	TexturePoint();
	TexturePoint(float xPos, float yPos);
	friend std::ostream &operator<<(std::ostream &os, const TexturePoint &point);

	TexturePoint operator+(const TexturePoint &other) const;
	TexturePoint operator-(const TexturePoint &other) const;

	template<typename N>
	TexturePoint operator+(const N num) const
	{
		return TexturePoint(x + num, y + num);
	}

	template<typename N>
	TexturePoint operator-(const N num) const
	{
		return TexturePoint(x - num, y - num);
	}

	template<typename N>
	TexturePoint operator*(const N num) const
	{
		return TexturePoint(x * num, y * num);
	}

	template<typename N>
	TexturePoint operator/(const N num) const
	{
		return TexturePoint(x / num, y / num);
	}
};

