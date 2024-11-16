#ifndef RASTERTEST_H
#define RASTERTEST_H

#include <chrono>

#include "RenderTest.h"
#include "../handlers/EventHandler.h"
#include "../handlers/FrameHandler.h"
#include "../renderers/RasterRenderer.h"
#include "../renderers/wrappers/RendererWrapper.h"

class RasterTest final : public RenderTest
{
public:
    std::vector<FrameHandler*> frameHandlers;
    std::vector<EventHandler*> eventHandlers;

    RasterTest();

    void handleEvent(SDL_Event& event, DrawingWindow& window) override;
    void renderFrame(DrawingWindow& window) override;

    void addRendererWrapper(RendererWrapper* rendererWrapper);

private:
    std::vector<RendererWrapper*> renderers;
    size_t currentTest = 0;
    std::chrono::milliseconds prevTime;

    RendererWrapper* getCurrentRendererWrapper() const;

    // Update previous time and return time difference in seconds.
    float updateDTime();
};


#endif //RASTERTEST_H
