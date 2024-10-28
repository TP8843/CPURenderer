#include "helper/Model.h"
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

    auto model = Model::import("textured-cornell-box.obj");
    auto camera = Camera(glm::vec3(0, 0, 10),
                         glm::mat3(),
                         5,
                         80);
    auto renderer = RasterRenderer2(model, camera);
    RenderTest* rasterTest = new RasterTest(renderer);
    renderLoop.addTest(rasterTest);

    DrawTests* drawTests = new DrawTests();
    renderLoop.addTest(drawTests);

    renderLoop.run();
}
