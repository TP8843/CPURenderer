#include "Model.h"
#include "RasterRenderer.h"
#include "RenderLoop.h"
#include "tests/DrawTests.h"
#include "tests/InteractiveTest.h"
#include "tests/RasterTest.h"

int main(int argc, char* argv[])
{
    auto renderLoop = RenderLoop();

    RenderTest* interactiveTest = new InteractiveTest();
    renderLoop.addTest(interactiveTest);

    auto model = Model::import("cornell-box.obj");
    auto renderer = RasterRenderer(model,
                                   glm::vec3(0, 0, 10),
                                   glm::mat3(1, 0, 0, 0, 1, 0, 0, 0, 1),
                                   5,
                                   80);
    RenderTest* rasterTest = new RasterTest(renderer);
    renderLoop.addTest(rasterTest);

    DrawTests* drawTests = new DrawTests();
    renderLoop.addTest(drawTests);

    renderLoop.run();
}
