//
// Created by Thomas Parr on 09/10/2024.
//

#include "DrawTests.h"

#include <CanvasPoint.h>
#include <Colour.h>
#include <DrawingWindow.h>

#include "../Draw.h"
#include "../Interpolation.h"

DrawTests::DrawTests(DrawingWindow &window) : window(window) {}

void DrawTests::drawRedNoise(DrawingWindow &window) {
	window.clearPixels();
	for (size_t y = 0; y < window.height; y++) {
		for (size_t x = 0; x < window.width; x++) {
			float red = rand() % 256;
			float green = 0.0;
			float blue = 0.0;
			uint32_t colour = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);
			window.setPixelColour(x, y, colour);
		}
	}
}

void DrawTests::drawGreyscaleInterpolation(DrawingWindow &window)
{
	window.clearPixels();
	std::vector<float> gradient = Interpolation::interpolateSingleFloats(255, 0, 256);

	for (float x = 0; x < window.width; x++)
	{
		const size_t index = floor((x / window.width) * gradient.size());
		const size_t color = gradient[index];
		for (size_t y = 0; y < window.height; y++)
		{
			window.setPixelColour(x, y, (color << 16) + (color << 8) + color);
		}
	}
}

void DrawTests::drawTwoDimensionalColorInterpolation(DrawingWindow &window)
{
	const glm::vec3 topLeft(255, 0, 0);        // red
	const glm::vec3 topRight(0, 0, 255);       // blue
	const glm::vec3 bottomRight(0, 255, 0);    // green
	const glm::vec3 bottomLeft(255, 255, 0);   // yellow

	window.clearPixels();

	const std::vector<glm::vec3> firstColumn = Interpolation::interpolateThreeElementValues(topLeft, bottomLeft, window.height);
	const std::vector<glm::vec3> lastColumn = Interpolation::interpolateThreeElementValues(topRight, bottomRight, window.height);

	for( size_t y = 0; y < window.height; y++)
	{
		std::vector<glm::vec3> row = Interpolation::interpolateThreeElementValues(firstColumn[y], lastColumn[y], window.width);
		for (size_t x = 0; x < window.width; x++)
		{
			const uint32_t color = (255 << 24) + (static_cast<uint32_t>(row[x].x) << 16) + (static_cast<uint32_t>(row[x].y) << 8) + static_cast<uint32_t>(row[x].z);
			window.setPixelColour(x, y, color);
		}
	}
}

void DrawTests::drawLineTest(DrawingWindow &window)
{
	const auto colour = Colour(255, 255, 255);

	auto from = CanvasPoint(window.width / 2, 0);
	auto to = CanvasPoint(window.width / 2, window.height - 1);
	Draw::drawLine(window, from, to, colour);

	from = CanvasPoint(0, 0);
	to = CanvasPoint(window.width / 2, window.height / 2);
	Draw::drawLine(window, from, to, colour);

	from = CanvasPoint(window.width - 1, 0);
	to = CanvasPoint(window.width / 2, window.height / 2);
	Draw::drawLine(window, from, to, colour);

	from = CanvasPoint(window.width / 3, window.height / 2);
	to = CanvasPoint(window.width * 2 / 3, window.height / 2);
	Draw::drawLine(window, from, to, colour);
}

void DrawTests::drawTexturedTriangleTest(DrawingWindow &window)
{
	const auto texture = TextureMap("./texture.ppm");
	const auto v1 = CanvasPoint(160, 10, TexturePoint(195, 5));
	const auto v2 = CanvasPoint(300, 230, TexturePoint(395, 380));
	const auto v3 = CanvasPoint(10, 150, TexturePoint(65, 330));

	const auto triangle = CanvasTriangle(v1, v2, v3);

	Draw::drawTexturedTriangle(window, triangle, texture);
	Draw::drawStrokedTriangle(window, triangle, Colour(255, 255, 255));
}

void DrawTests::drawEdgeCaseTriangles (DrawingWindow &window)
{
	const auto colour = Colour(255, 255, 255);
	const auto triangle1 = CanvasTriangle(
		CanvasPoint(0, 0),
		CanvasPoint(100, 0),
		CanvasPoint(50, 200)
	);

	const auto triangle2 = CanvasTriangle(
		CanvasPoint(window.width -1, 0),
		CanvasPoint(window.width - 1, 100),
		CanvasPoint(window.width - 50, 100)
	);

	const auto triangle3 = CanvasTriangle(
		CanvasPoint(window.width / 2, window.height - 51),
		CanvasPoint(window.width / 2 + 100, window.height - 1),
		CanvasPoint(window.width / 2, window.height - 1)
	);

	const auto triangle4 = CanvasTriangle(
	CanvasPoint(window.width / 2, 20),
	CanvasPoint(window.width / 2 + 100, 20),
	CanvasPoint(window.width / 2, 21)
);

	Draw::drawFilledTriangle(window, triangle1, colour);
	Draw::drawFilledTriangle(window, triangle2, colour);
	Draw::drawFilledTriangle(window, triangle3, colour);
	Draw::drawFilledTriangle(window, triangle4, colour);
}

void DrawTests::handleEvent(const SDL_Event& event, DrawingWindow& window)
{
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
		else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
		else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
		else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
		else if (event.key.keysym.sym == SDLK_t)
		{
			currentTest = (currentTest + 1) % testFunctions.size();
			window.clearPixels();
		}
		else if (event.key.keysym.sym == SDLK_u)
		{
			const auto triangle = CanvasTriangle(
				CanvasPoint(rand() % window.width, rand() % window.height),
				CanvasPoint(rand() % window.width, rand() % window.height),
				CanvasPoint(rand() % window.width, rand() % window.height)
			);

			const auto colour = Colour(rand() % 255, rand() % 255, rand() % 255);
			const auto white = Colour(255, 255, 255);

			Draw::drawFilledTriangle(window, triangle, colour);
			Draw::drawStrokedTriangle(window, triangle, white);
		}
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
	}
}


void DrawTests::run()
{
	SDL_Event event;

	while(true)
	{
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window);

		testFunctions[currentTest](window);

		window.renderFrame();
	}
}
