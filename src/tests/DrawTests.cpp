#include "DrawTests.h"

#include <CanvasPoint.h>
#include <Colour.h>
#include <DrawingWindow.h>

#include "../helper/constants.h"
#include "../helper/Draw.h"
#include "../helper/Interpolation.h"
#include "../objects/materials/MaterialMap.h"

DrawTests::DrawTests() : currentTest(0) {}

void DrawTests::drawRedNoise(DrawingWindow &window) {
	window.clearPixels();
	for (size_t y = 0; y < window.height; y++) {
		for (size_t x = 0; x < window.width; x++) {
			const float red = rand() % 256;
			constexpr float green = 0.0;
			constexpr float blue = 0.0;
			const uint32_t colour = (255 << 24) + (static_cast<int>(red) << 16) + (static_cast<int>(green) << 8) + static_cast<int>(blue);
			window.setPixelColour(x, y, colour);
		}
	}
}

void DrawTests::drawGreyscaleInterpolation(DrawingWindow &window)
{
	window.clearPixels();
	const std::vector<float> gradient = Interpolation::interpolateSingleFloats(255, 0, 256);

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

	const auto triangle = CanvasTriangle(v1, v2, v3, Colour(255, 255, 255));

	Draw::drawTexturedTriangle(window, triangle, texture);
	Draw::drawStrokedTriangle(window, triangle);
}

void DrawTests::drawEdgeCaseTriangles (DrawingWindow &window)
{
	const auto colour = Colour(255, 255, 255);
	const auto triangle1 = CanvasTriangle(
		CanvasPoint(0, 0),
		CanvasPoint(100, 0),
		CanvasPoint(50, 200),
		Colour(255, 255, 255)
	);

	const auto triangle2 = CanvasTriangle(
		CanvasPoint(window.width -1, 0),
		CanvasPoint(window.width - 1, 100),
		CanvasPoint(window.width - 50, 100),
		Colour(255, 255, 255)
	);

	const auto triangle3 = CanvasTriangle(
		CanvasPoint(window.width / 2, window.height - 51),
		CanvasPoint(window.width / 2 + 100, window.height - 1),
		CanvasPoint(window.width / 2, window.height - 1),
		Colour(255, 255, 255)
	);

	const auto triangle4 = CanvasTriangle(
	CanvasPoint(window.width / 2, 20),
	CanvasPoint(window.width / 2 + 100, 20),
	CanvasPoint(window.width / 2, 21),
	Colour(255, 255, 255)
);

	Draw::drawFilledTriangle(window, triangle1);
	Draw::drawFilledTriangle(window, triangle2);
	Draw::drawFilledTriangle(window, triangle3);
	Draw::drawFilledTriangle(window, triangle4);
}

void DrawTests::drawNoiseTest(DrawingWindow &window)
{
	auto materialMap = MaterialMap();

	for (int i = 0; i < 100; i++)
	{
		glm::vec2 random = materialMap.getSampledVec2(0, 0, i);
		random = random * 100.f + glm::vec2(window.width / 2.f,window.height / 2.f);

		const auto randomOffset = materialMap.getSampledVec2(0, 0, i);
		const auto pointRadius = (1.f / 3.f) * randomOffset.x;
		const auto pointAngle = randomOffset.y * 2 * M_PI;
		auto discPos = glm::vec2(pointRadius * glm::cos(pointAngle), pointRadius * glm::sin(pointAngle));
		discPos = discPos * 200.f + glm::vec2(window.width / 2, window.height / 2);

		window.setPixelColour(discPos.x, discPos.y, 0xFFFFFFFF);
	}
}

void DrawTests::handleEvent(SDL_Event& event, DrawingWindow& window)
{
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == constants::keyboard::SUB_TEST)
		{
			currentTest = (currentTest + 1) % testFunctions.size();
			window.clearPixels();
		}
		else if (event.key.keysym.sym == constants::keyboard::RAND_TRIANGLE)
		{
			const auto triangle = CanvasTriangle(
				CanvasPoint(rand() % window.width, rand() % window.height),
				CanvasPoint(rand() % window.width, rand() % window.height),
				CanvasPoint(rand() % window.width, rand() % window.height),
				Colour(rand() % 255, rand() % 255, rand() % 255)
			);

			const auto outline = CanvasTriangle(
				triangle[0], triangle[1], triangle[2], Colour(255, 255, 255)
			);

			Draw::drawFilledTriangle(window, triangle);
			Draw::drawStrokedTriangle(window, outline);
		}
	}
}


void DrawTests::renderFrame(DrawingWindow &window)
{
	testFunctions[currentTest](window);
}
