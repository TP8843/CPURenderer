//
// Created by Thomas Parr on 09/10/2024.
//

#ifndef RASTERTEST_H
#define RASTERTEST_H
#include "RenderTest.h"
#include "../RasterRenderer2.h"


class RasterTest : public RenderTest
{
public:
    static void drawPointCloud(RasterRenderer2 &renderer, DrawingWindow& window);
    static void drawWireframe(RasterRenderer2 &renderer, DrawingWindow& window);
    static void drawRaster(RasterRenderer2 &renderer, DrawingWindow& window);

    explicit RasterTest(RasterRenderer2 &renderer);

    void handleEvent(SDL_Event& event, DrawingWindow& window) override;
    void renderFrame(DrawingWindow& window) override;

private:
    RasterRenderer2 &renderer;
    int currentTest = 1;

    std::vector<void (*)(RasterRenderer2 &renderer, DrawingWindow& window)> testFunctions
        = std::vector<void (*)(RasterRenderer2 &renderer, DrawingWindow& window)>
        {
            drawPointCloud,
            drawWireframe,
            drawRaster
        };
};


#endif //RASTERTEST_H
