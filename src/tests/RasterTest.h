//
// Created by Thomas Parr on 09/10/2024.
//

#ifndef RASTERTEST_H
#define RASTERTEST_H

#include "RenderTest.h"
#include "../renderers/RasterRenderer2.h"
#include "../renderers/RendererWrapper.h"

class RasterTest : public RenderTest
{
public:
    RasterTest();

    void handleEvent(SDL_Event& event, DrawingWindow& window) override;
    void renderFrame(DrawingWindow& window) override;

    void addRendererWrapper(RendererWrapper* rendererWrapper);

private:
    std::vector<RendererWrapper*> renderers;
    int currentTest = 0;

    RendererWrapper* getCurrentRendererWrapper() const;

    void processKeys() const;
};


#endif //RASTERTEST_H
