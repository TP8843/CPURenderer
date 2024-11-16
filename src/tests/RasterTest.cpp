#include "RasterTest.h"

#include "../helper/constants.h"
#include "../renderers/RasterRenderer.h"

using namespace constants;

RasterTest::RasterTest() :
    renderers(std::vector<RendererWrapper*>()),
    prevTime(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()))
{}

void RasterTest::handleEvent(SDL_Event& event, DrawingWindow& window)
{
    for (const auto eventHandler: eventHandlers)
    {
        eventHandler->handleEvent(window, event);
    }

    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == keyboard::SUB_TEST)
        {
            currentTest = (currentTest + 1) % renderers.size();
            window.clearPixels();
        }
    }
}

void RasterTest::renderFrame(DrawingWindow &window)
{
    RendererWrapper* renderer = getCurrentRendererWrapper();

    window.clearPixels();

    const auto deltaTime = updateDTime();

    for (const auto frameHandler : preFrameHandlers)
    {
        frameHandler->handleFrame(window, deltaTime);
    }

    renderer->renderFrame(window);

    for (const auto frameHandler : postFrameHandlers)
    {
        frameHandler->handleFrame(window, deltaTime);
    }
}

void RasterTest::addRendererWrapper(RendererWrapper* rendererWrapper)
{
    renderers.emplace_back(rendererWrapper);
}

RendererWrapper* RasterTest::getCurrentRendererWrapper() const
{
    return renderers.at(currentTest);
}

float RasterTest::updateDTime()
{
    const std::chrono::milliseconds time = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch());

    const float timeElapsed = static_cast<float>((time - prevTime).count()) / 1000.0f;

    prevTime = std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::system_clock::now().time_since_epoch());

    return timeElapsed;
}
