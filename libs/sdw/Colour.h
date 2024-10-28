#pragma once

#include <iostream>

struct Colour {
	std::string name;
	int red{};
	int green{};
	int blue{};
	Colour();
	Colour(int r, int g, int b);
	Colour(std::string n, int r, int g, int b);

	int asARGB() const
	{
		return (255 << 24) + (red << 16) + (green << 8) + blue;
	};

	Colour operator+(const Colour &other) const;
	Colour operator-(const Colour &other) const;

	template<typename N>
	Colour operator*(const N num)
	{
		return Colour(name, red * num, green * num, blue * num);
	}
};

std::ostream &operator<<(std::ostream &os, const Colour &colour);