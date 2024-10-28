#include "RasterTest.h"

#include "../helper/constants.h"
#include "../RasterRenderer2.h"

using namespace constants;

RasterTest::RasterTest(RasterRenderer2 &renderer)
    : renderer(renderer)
{
}

void RasterTest::drawPointCloud(RasterRenderer2 &renderer, DrawingWindow &window)
{
    renderer.pointCloudRender(window);
}

void RasterTest::drawWireframe(RasterRenderer2 &renderer, DrawingWindow &window)
{
    renderer.wireframeRender(window);
}

void RasterTest::drawRaster(RasterRenderer2 &renderer, DrawingWindow &window)
{
    renderer.rasterRender(window);
}

void RasterTest::handleEvent(SDL_Event& event, DrawingWindow& window)
{
    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == keyboard::SUB_TEST)
        {
            currentTest = (currentTest + 1) % testFunctions.size();
            window.clearPixels();
        }

        // Pedestal camera up
        if (event.key.keysym.sym == keyboard::PEDESTAL_UP)
        {
            renderer.camera.translateRelative(glm::vec3(0, speed::TRANSLATION_SPEED, 0));
        }

        // Pedestal camera down
        if (event.key.keysym.sym == keyboard::PEDESTAL_DOWN)
        {
            renderer.camera.translateRelative(glm::vec3(0, -speed::TRANSLATION_SPEED, 0));
        }

        // Dolly camera "forwards" in camera space
        if (event.key.keysym.sym == keyboard::DOLLY_FORWARD)
        {
            renderer.camera.translateRelative(glm::vec3(0, 0, speed::TRANSLATION_SPEED));
        }
        // Dolly camera "backwards"
        if (event.key.keysym.sym == keyboard::DOLLY_BACK)
        {
            renderer.camera.translateRelative(glm::vec3(0, 0, -speed::TRANSLATION_SPEED));
        }

        // Truck camera left
        if (event.key.keysym.sym == keyboard::TRUCK_LEFT)
        {
            renderer.camera.translateRelative(glm::vec3(-speed::TRANSLATION_SPEED, 0, 0));
        }

        // Truck camera right
        if (event.key.keysym.sym == keyboard::TRUCK_RIGHT)
        {
            renderer.camera.translateRelative(glm::vec3(speed::TRANSLATION_SPEED, 0, 0));
        }

        // Pan left
        if (event.key.keysym.sym == keyboard::PAN_LEFT)
        {
            renderer.camera.rotateY(speed::ROTATION_SPEED);
        }

        // Pan right
        if (event.key.keysym.sym == keyboard::PAN_RIGHT)
        {
            renderer.camera.rotateY(-speed::ROTATION_SPEED);
        }

        // Tilt up
        if (event.key.keysym.sym == keyboard::TILT_UP)
        {
            renderer.camera.rotateX(speed::ROTATION_SPEED);
        }

        // Tilt down
        if (event.key.keysym.sym == keyboard::TILT_DOWN)
        {
            renderer.camera.rotateX(-speed::ROTATION_SPEED);
        }

        // Toggle orbit mode
        if (event.key.keysym.sym == keyboard::ORBIT)
        {
            renderer.camera.toggleOrbit();
        }

        if (event.key.keysym.sym == keyboard::RESET_RENDER)
        {
            renderer.camera.reset();
        }

        if (event.key.keysym.sym == keyboard::LOOK_AT)
        {
            renderer.camera.lookAt(glm::vec3(0, 0, 0));
        }
    }
}

void RasterTest::renderFrame(DrawingWindow &window)
{
    window.clearPixels();
    testFunctions[currentTest](renderer, window);

    if (renderer.camera.getOrbit())
    {
        renderer.camera.iterateOrbit();
    }
}