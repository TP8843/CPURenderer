//
// Created by Thomas Parr on 09/10/2024.
//

#ifndef RASTERTEST_H
#define RASTERTEST_H

#include "RenderTest.h"
#include "../renderers/RasterRenderer.h"
#include "../renderers/RendererWrapper.h"

class RasterTest final : public RenderTest
{
public:
    RasterTest();

    void handleEvent(SDL_Event& event, DrawingWindow& window) override;
    void renderFrame(DrawingWindow& window) override;

    void addRendererWrapper(RendererWrapper* rendererWrapper);

private:
    std::vector<RendererWrapper*> renderers;
    size_t currentTest = 0;
    std::chrono::milliseconds prevTime;

    RendererWrapper* getCurrentRendererWrapper() const;

    void processKeys();
};


#endif //RASTERTEST_H
