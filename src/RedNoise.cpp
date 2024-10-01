#include <CanvasTriangle.h>
#include <Colour.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>

#include "Draw.h"
#include "Interpolation.h"

#define WIDTH 320
#define HEIGHT 240

void drawRedNoise(DrawingWindow &window) {
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

void drawGreyscaleInterpolation(DrawingWindow &window)
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

void drawTwoDimensionalColorInterpolation(DrawingWindow &window)
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

void drawLineTest(DrawingWindow &window)
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

void handleEvent(SDL_Event event, DrawingWindow &window) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
		else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
		else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
		else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
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
	} else if (event.type == SDL_MOUSEBUTTONDOWN) {
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
	}
}

int main(int argc, char *argv[]) {
	const std::vector<glm::vec3> result = Interpolation::interpolateThreeElementValues(
		glm::vec3(1.0, 4.0, 9.2),
		glm::vec3(4.0, 1.0, 9.8),
		4);

	for(size_t i = 0; i < result.size(); i++)
		std::cout << "(" << result[i].x << ", " << result[i].y << ", " << result[i].z << ")" << std::endl;

	std::cout << std::endl;

	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;
	while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window);

		// drawRedNoise(window);
		// drawGreyscaleInterpolation(window);
		// drawTwoDimensionalColorInterpolation(window);
		// drawLineTest(window);

		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}
}
