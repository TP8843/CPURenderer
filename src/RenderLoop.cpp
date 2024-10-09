//
// Created by Thomas Parr on 09/10/2024.
//

#include "RenderLoop.h"

#include <DrawingWindow.h>
#include <SDL_events.h>

#include "Draw.h"

#define WIDTH 640
#define HEIGHT 480

RenderLoop::RenderLoop()
    : window(DrawingWindow(WIDTH, HEIGHT, false)),
      currentTest(0),
      tests(std::vector<RenderTest*>())
{
}

void RenderLoop::handleEvent(SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
        else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
        else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
        else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
        else if (event.key.keysym.sym == SDLK_m)
        {
            std::cout << "Changing to test " << currentTest << std::endl;

            currentTest = (currentTest + 1) % tests.size();
            window.clearPixels();
        }
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        window.savePPM("output.ppm");
        window.saveBMP("output.bmp");
    }

    tests[currentTest]->handleEvent(event, window);
}

void RenderLoop::addTest(RenderTest* test)
{
    tests.push_back(test);
}


void RenderLoop::run()
{
    SDL_Event event;


    while (true)
    {
        // We MUST poll for events - otherwise the window will freeze !
        if (window.pollForInputEvents(event)) handleEvent(event);


        getCurrentRenderTest()->renderFrame(window);

        window.renderFrame();
    }
}

RenderTest* RenderLoop::getCurrentRenderTest()
{
    return tests[currentTest];
}