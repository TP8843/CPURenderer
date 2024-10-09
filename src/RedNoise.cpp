#include <DrawingWindow.h>

#include "Model.h"
#include "RasterRenderer.h"
#include "RenderLoop.h"
#include "tests/DrawTests.h"
#include "tests/InteractiveTest.h"
#include "tests/RasterTest.h"

#define WIDTH 640
#define HEIGHT 480

int main(int argc, char* argv[])
{
    auto window = DrawingWindow(WIDTH, HEIGHT, false);
    auto renderLoop = RenderLoop();

    RenderTest* interactiveTest = new InteractiveTest(window);
    renderLoop.addTest(interactiveTest);

    auto model = Model::import("cornell-box.obj");
    auto renderer = RasterRenderer(model,
                                   glm::vec3(0, 0, 10),
                                   5,
                                   80);
    RenderTest* rasterTest = new RasterTest(renderer);
    renderLoop.addTest(rasterTest);

    DrawTests* drawTests = new DrawTests();
    renderLoop.addTest(drawTests);

    renderLoop.run();
}
