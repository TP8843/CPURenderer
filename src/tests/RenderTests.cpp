//
// Created by Thomas Parr on 09/10/2024.
//

#include "RenderTests.h"

RenderTests::RenderTests(DrawingWindow &window, RasterRenderer &renderer)
    : window(window), renderer(renderer)
{
}

void RenderTests::drawPointCloud(RasterRenderer &renderer)
{
    renderer.pointCloudRender();
}

void RenderTests::drawWireframe(RasterRenderer &renderer)
{
    renderer.wireframeRender();
}

void RenderTests::drawRaster(RasterRenderer &renderer)
{
    renderer.rasterRender();
}

void RenderTests::handleEvent(const SDL_Event& event, DrawingWindow& window)
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
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        window.savePPM("output.ppm");
        window.saveBMP("output.bmp");
    }
}

void RenderTests::run()
{
    SDL_Event event;

    while (true)
    {
        // We MUST poll for events - otherwise the window will freeze !
        if (window.pollForInputEvents(event)) handleEvent(event, window);

        testFunctions[currentTest](renderer);

        window.renderFrame();
    }
}
