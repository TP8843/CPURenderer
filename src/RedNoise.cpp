#include "objects/Model.h"
#include "renderers/RasterRenderer2.h"
#include "RenderLoop.h"
#include "renderers/RendererWrapper.h"
#include "renderers/RasterWrapper.h"
#include "renderers/RaytracerWrapper.h"
#include "renderers/WireframeWrapper.h"
#include "tests/DrawTests.h"
#include "tests/InteractiveTest.h"
#include "tests/RasterTest.h"

int main(int argc, char* argv[])
{
    auto renderLoop = RenderLoop();

    auto transformation = Transformation();
    transformation.scale = 1.f;
    transformation.position = glm::vec3(0, 2, 0);
    auto model = Model::import("../models/textured-cornell-box.obj", transformation);
    auto camera = Camera(glm::vec3(0, 0, 10),
                         glm::mat3(),
                         1,
                         480);
    auto light = Light(glm::vec3(0, 2, 0), 20.0f);

    auto renderer = RasterRenderer2(model, camera, light);
    auto* rasterWrapper = new RasterWrapper(renderer);
    auto* wireframeWrapper = new WireframeWrapper(renderer);

    auto raytracer = Raytracer(model, camera, light);
    auto* raytracerWrapper = new RaytracerWrapper(raytracer);

    auto* rasterTest = new RasterTest();
    rasterTest->addRendererWrapper(rasterWrapper);
    rasterTest->addRendererWrapper(wireframeWrapper);
    rasterTest->addRendererWrapper(raytracerWrapper);

    renderLoop.addTest(rasterTest);

    RenderTest* interactiveTest = new InteractiveTest();
    renderLoop.addTest(interactiveTest);


    DrawTests* drawTests = new DrawTests();
    renderLoop.addTest(drawTests);

    renderLoop.run();
}
