//
// Created by Thomas Parr on 09/10/2024.
//

#include "InteractiveTest.h"

#include <../../libs/sdw/CanvasTriangle.h>

#include "../Draw.h"

InteractiveTest::InteractiveTest(DrawingWindow& window)
    : window(window),
      selectedVertex(3),
      triangle(CanvasTriangle(CanvasPoint(window.width / 3, window.height * 2 / 3),
                              CanvasPoint(window.width * 2 / 3, window.height * 2 / 3),
                              CanvasPoint(window.width / 2, window.height / 3))),
      triangleColour(Colour(255, 255, 255)),
      vertexColour(Colour(255, 0, 0))
{
}

void InteractiveTest::handleEvent(SDL_Event& event, DrawingWindow& window)
{
    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_t)
        {
            selectedVertex = (selectedVertex + 1) % 4;
        }
    }

    // If vertex is currently selected
    if (selectedVertex < 3)
    {
        int x;
        int y;
        SDL_GetMouseState(&x, &y);

        triangle.vertices[selectedVertex].x = x;
        triangle.vertices[selectedVertex].y = y;
    }
}


void InteractiveTest::renderFrame(DrawingWindow &window)
{
    window.clearPixels();

    Draw::drawFilledTriangle(window, triangle, triangleColour);

    if (selectedVertex < 3)
    {
        const auto currentVertex = triangle.vertices[selectedVertex];

        for (int i = -4; i <= 4; i++)
        {
            for (int j = -4; j <= 4; j++)
            {
                window.setPixelColour(currentVertex.x + i, currentVertex.y + j, 0xFFFF0000);
            }
        }
    }
}
