//
// Created by Thomas Parr on 09/10/2024.
//

#include "RasterTest.h"

RasterTest::RasterTest(RasterRenderer &renderer)
    : renderer(renderer)
{
}

void RasterTest::drawPointCloud(RasterRenderer &renderer, DrawingWindow &window)
{
    renderer.pointCloudRender(window);
}

void RasterTest::drawWireframe(RasterRenderer &renderer, DrawingWindow &window)
{
    renderer.wireframeRender(window);
}

void RasterTest::drawRaster(RasterRenderer &renderer, DrawingWindow &window)
{
    renderer.rasterRender(window);
}

void RasterTest::handleEvent(SDL_Event& event, DrawingWindow& window)
{
    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_t)
        {
            currentTest = (currentTest + 1) % testFunctions.size();
            window.clearPixels();
        }
    }
}

void RasterTest::renderFrame(DrawingWindow &window)
{
    testFunctions[currentTest](renderer, window);
}