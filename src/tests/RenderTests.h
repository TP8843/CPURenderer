//
// Created by Thomas Parr on 09/10/2024.
//

#ifndef RENDERTESTS_H
#define RENDERTESTS_H
#include <../../libs/sdw/DrawingWindow.h>

#include "../Model.h"
#include "../RasterRenderer.h"


class RenderTests {
public:
    static void drawPointCloud(RasterRenderer &renderer);
    static void drawWireframe(RasterRenderer &renderer);
    static void drawRaster(RasterRenderer &renderer);

    explicit RenderTests(DrawingWindow &window, RasterRenderer &renderer);

    void run();

private:
    void handleEvent(const SDL_Event& event, DrawingWindow& window);

    DrawingWindow& window;
    RasterRenderer &renderer;
    int currentTest = 1;

    std::vector<void (*)(RasterRenderer &renderer)> testFunctions = std::vector<void (*)(RasterRenderer &renderer)>
    {
        drawPointCloud,
        drawWireframe,
        drawRaster
    };
};



#endif //RENDERTESTS_H
