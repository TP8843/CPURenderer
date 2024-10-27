//
// Created by Thomas Parr on 09/10/2024.
//

#include "InteractiveTest.h"

#include <../../libs/glm-0.9.7.2/glm/glm.hpp>
#include <../../libs/sdw/CanvasTriangle.h>

#include "../helper/constants.h"
#include "../helper/Draw.h"

InteractiveTest::InteractiveTest()
    : selectedVertex(3),
      triangle(CanvasTriangle(CanvasPoint(20, 100),
                              CanvasPoint(100, 100),
                              CanvasPoint(60, 50),
                              Colour(255, 255, 255))),
      triangleColour(Colour(255, 255, 255)),
      vertexColour(Colour(255, 0, 0))
{
}

void InteractiveTest::handleEvent(SDL_Event& event, DrawingWindow& window)
{
    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == constants::keyboard::SUB_TEST)
        {
            selectedVertex = (selectedVertex + 1) % 4;
        }
    }

    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            int x;
            int y;
            SDL_GetMouseState(&x, &y);

            if (selectedVertex == 3)
            {
                for (int i = 0; i < 3; i++)
                {
                    const auto vertex = triangle.vertices[i];

                    if (glm::abs(vertex.x - x) < 10 && glm::abs(vertex.y - y) < 10)
                    {
                        selectedVertex = i;
                    }
                }
            }
            else
            {
                selectedVertex = 3;
            }

        }
    }
}


void InteractiveTest::renderFrame(DrawingWindow &window)
{
    window.clearPixels();

    // If vertex is currently selected, track mouse position
    if (selectedVertex < 3)
    {
        int x;
        int y;
        SDL_GetMouseState(&x, &y);

        triangle.vertices[selectedVertex].x = x;
        triangle.vertices[selectedVertex].y = y;
    }

    Draw::drawFilledTriangle(window, triangle);

    for (int v = 0; v < 3; v++)
    {
        const auto currentVertex = triangle.vertices[v];
        const auto colour = (selectedVertex == v) ? 0xFFFF0000 : 0xFF7F0000;

        for (int i = -6; i <= 6; i++)
        {
            for (int j = -6; j <= 6; j++)
            {
                window.setPixelColour(currentVertex.x + i, currentVertex.y + j, colour);
            }
        }
    }
}
