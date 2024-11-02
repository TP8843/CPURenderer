#include "RasterTest.h"

#include "../helper/constants.h"
#include "../renderers/RasterRenderer2.h"

using namespace constants;

RasterTest::RasterTest() :
    renderers(std::vector<RendererWrapper*>()),
    prevTime(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()))
{}

void RasterTest::handleEvent(SDL_Event& event, DrawingWindow& window)
{
    RendererWrapper* renderer = getCurrentRendererWrapper();

    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == keyboard::SUB_TEST)
        {
            currentTest = (currentTest + 1) % renderers.size();
            window.clearPixels();
        }

        // Toggle orbit mode
        if (event.key.keysym.sym == keyboard::ORBIT)
        {
            renderer->getCamera().toggleOrbit();
        }

        if (event.key.keysym.sym == keyboard::RESET_RENDER)
        {
            renderer->getCamera().reset();
        }

        if (event.key.keysym.sym == keyboard::LOOK_AT)
        {
            renderer->getCamera().lookAt(glm::vec3(0, 0, 0));
        }
    }
}

void RasterTest::renderFrame(DrawingWindow &window)
{
    RendererWrapper* renderer = getCurrentRendererWrapper();

    window.clearPixels();
    processKeys();

    if (renderer->getCamera().getOrbit())
    {
        renderer->getCamera().iterateOrbit();
    }

    renderer->renderFrame(window);
}

void RasterTest::addRendererWrapper(RendererWrapper* rendererWrapper)
{
    renderers.emplace_back(rendererWrapper);
}

RendererWrapper* RasterTest::getCurrentRendererWrapper() const
{
    return renderers.at(currentTest);
}

void RasterTest::processKeys()
{
    RendererWrapper* renderer = getCurrentRendererWrapper();
    const Uint8* keys = SDL_GetKeyboardState(nullptr);

    const std::chrono::milliseconds currentTime =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

    const float timeElapsed = (currentTime - prevTime).count() / 1000.0f;

    // Pedestal camera up
    if (keys[keyboard::PEDESTAL_UP])
    {
        renderer->getCamera().translateRelative(glm::vec3(0, speed::TRANSLATION_SPEED * timeElapsed, 0));
    }

    // Pedestal camera down
    if (keys[keyboard::PEDESTAL_DOWN])
    {
        renderer->getCamera().translateRelative(glm::vec3(0, -speed::TRANSLATION_SPEED * timeElapsed, 0));
    }

    // Dolly camera "forwards" in camera space
    if (keys[keyboard::DOLLY_FORWARD])
    {
        renderer->getCamera().translateRelative(glm::vec3(0, 0, speed::TRANSLATION_SPEED * timeElapsed));
    }
    // Dolly camera "backwards"
    if (keys[keyboard::DOLLY_BACK])
    {
        renderer->getCamera().translateRelative(glm::vec3(0, 0, -speed::TRANSLATION_SPEED * timeElapsed));
    }

    // Truck camera left
    if (keys[keyboard::TRUCK_LEFT])
    {
        renderer->getCamera().translateRelative(glm::vec3(-speed::TRANSLATION_SPEED * timeElapsed, 0, 0));
    }

    // Truck camera right
    if (keys[keyboard::TRUCK_RIGHT])
    {
        renderer->getCamera().translateRelative(glm::vec3(speed::TRANSLATION_SPEED * timeElapsed, 0, 0));
    }

    // Pan left
    if (keys[keyboard::PAN_LEFT])
    {
        renderer->getCamera().rotateY(speed::ROTATION_SPEED * timeElapsed);
    }

    // Pan right
    if (keys[keyboard::PAN_RIGHT])
    {
        renderer->getCamera().rotateY(-speed::ROTATION_SPEED * timeElapsed);
    }

    // Tilt up
    if (keys[keyboard::TILT_UP])
    {
        renderer->getCamera().rotateX(speed::ROTATION_SPEED * timeElapsed);
    }

    // Tilt down
    if (keys[keyboard::TILT_DOWN])
    {
        renderer->getCamera().rotateX(-speed::ROTATION_SPEED * timeElapsed);
    }

    prevTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
}
