#include <DrawingWindow.h>

#include "Model.h"
#include "tests/DrawTests.h"
#include "tests/InteractiveTest.h"
#include "tests/RenderTests.h"

#define WIDTH 640
#define HEIGHT 480

int main(int argc, char* argv[])
{
    auto window = DrawingWindow(WIDTH, HEIGHT, false);

    auto interactiveTest = InteractiveTest(window);
    interactiveTest.run();

    // auto model = Model::import("cornell-box.obj");
    // auto renderer = RasterRenderer(window,
    //                                model,
    //                                glm::vec3(0, 0, 10),
    //                                5,
    //                                80);
    // auto renderTests = RenderTests(window, renderer);
    // renderTests.run();

    // auto drawTests = DrawTests(window);
    // drawTests.run();
}
