//
// Created by Thomas Parr on 09/10/2024.
//

#ifndef RASTERTEST_H
#define RASTERTEST_H
#include "RenderTest.h"
#include "../RasterRenderer.h"


class RasterTest : public RenderTest
{
public:
    static void drawPointCloud(RasterRenderer& renderer, DrawingWindow& window);
    static void drawWireframe(RasterRenderer& renderer, DrawingWindow& window);
    static void drawRaster(RasterRenderer& renderer, DrawingWindow& window);

    explicit RasterTest(RasterRenderer& renderer);

    void handleEvent(SDL_Event& event, DrawingWindow& window) override;
    void renderFrame(DrawingWindow& window) override;

private:
    RasterRenderer& renderer;
    int currentTest = 1;

    std::vector<void (*)(RasterRenderer& renderer, DrawingWindow& window)> testFunctions
        = std::vector<void (*)(RasterRenderer& renderer, DrawingWindow& window)>
        {
            drawPointCloud,
            drawWireframe,
            drawRaster
        };
};


#endif //RASTERTEST_H
