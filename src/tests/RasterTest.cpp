//
// Created by Thomas Parr on 09/10/2024.
//

#include "RasterTest.h"
#define TRANSLATION_RATE 0.05
#define ROTATION_RATE 0.05

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

        // Tripod camera up
        if (event.key.keysym.sym == SDLK_r)
        {
            renderer.cameraPosition += renderer.cameraRotation *  glm::vec3(0, TRANSLATION_RATE, 0);
        }

        // Tripod camera down
        if (event.key.keysym.sym == SDLK_f)
        {
            renderer.cameraPosition += renderer.cameraRotation *  glm::vec3(0, -TRANSLATION_RATE, 0);
        }

        // Dolly camera "forwards"
        if (event.key.keysym.sym == SDLK_s)
        {
            renderer.cameraPosition += renderer.cameraRotation * glm::vec3(0, 0, TRANSLATION_RATE);
        }

        // Dolly camera "backwards"
        if (event.key.keysym.sym == SDLK_w)
        {
            renderer.cameraPosition += renderer.cameraRotation * glm::vec3(0, 0, -TRANSLATION_RATE);
        }

        // Pan camera left
        if (event.key.keysym.sym == SDLK_a)
        {
            renderer.cameraPosition += renderer.cameraRotation * glm::vec3(-TRANSLATION_RATE, 0, 0);
        }

        // Pan camera right about y-axis
        if (event.key.keysym.sym == SDLK_d)
        {
            renderer.cameraPosition += renderer.cameraRotation *  glm::vec3(TRANSLATION_RATE, 0, 0);
        }

        // Rotate camera left about y-axis
        if (event.key.keysym.sym == SDLK_q)
        {
            renderer.cameraRotation = renderer.cameraRotation
                * glm::mat3(glm::cos(-ROTATION_RATE), 0, glm::sin(-ROTATION_RATE), 0, 1, 0, -glm::sin(-ROTATION_RATE), 0, glm::cos(-ROTATION_RATE));
        }

        // Rotate camera right about y-axis
        if (event.key.keysym.sym == SDLK_e)
        {
            renderer.cameraRotation = renderer.cameraRotation
                * glm::mat3(glm::cos(ROTATION_RATE), 0, glm::sin(ROTATION_RATE), 0, 1, 0, -glm::sin(ROTATION_RATE), 0, glm::cos(ROTATION_RATE));
        }
    }
}

void RasterTest::renderFrame(DrawingWindow &window)
{
    window.clearPixels();
    testFunctions[currentTest](renderer, window);
}