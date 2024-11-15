#include "objects/Model.h"
#include "renderers/RasterRenderer.h"
#include "RenderLoop.h"
#include "handlers/CameraControl.h"
#include "handlers/OrbitHandler.h"
#include "renderers/RendererWrapper.h"
#include "renderers/RasterWrapper.h"
#include "renderers/RaytracerWrapper.h"
#include "renderers/WireframeWrapper.h"
#include "tests/DrawTests.h"
#include "tests/InteractiveTest.h"
#include "tests/RasterTest.h"

#define WIDTH 640
#define HEIGHT 480

int main(int argc, char* argv[])
{
    char* modelFile = "../models/textured-cornell-box.obj";
    float scale = 1.0f;
    if (argc > 1)
    {
        modelFile = argv[1];
    }

    if (argc > 2)
    {
        scale = std::stof(argv[2]);
    }


    auto renderLoop = RenderLoop(WIDTH, HEIGHT);

    auto transformation = Transformation();
    transformation.scale = scale;
    auto model = Model::import(modelFile, transformation);

    auto camera = Transformation(glm::vec3(0, 0, 10),
                         glm::mat3(),
                         1);

    auto light = Transformation(glm::vec3(0, 2, 0), glm::mat3(), 20.0f);

    auto orbitHandler = OrbitHandler(camera, glm::vec3());
    auto cameraControl = CameraControl(camera, light);



    auto renderer = RasterRenderer(model, camera, light);
    auto rasterWrapper = RasterWrapper(renderer);
    auto wireframeWrapper = WireframeWrapper(renderer);

    auto raytracer = Raytracer(model, camera, light);
    auto raytracerWrapper = RaytracerWrapper(raytracer);

    auto rasterTest = RasterTest();
    rasterTest.addRendererWrapper(&rasterWrapper);
    rasterTest.addRendererWrapper(&wireframeWrapper);
    rasterTest.addRendererWrapper(&raytracerWrapper);

    rasterTest.frameHandlers.push_back(&orbitHandler);
    rasterTest.eventHandlers.push_back(&orbitHandler);
    rasterTest.frameHandlers.push_back(&cameraControl);

    renderLoop.addTest(&rasterTest);

    auto interactiveTest = InteractiveTest();
    renderLoop.addTest(&interactiveTest);


    auto drawTests = DrawTests();
    renderLoop.addTest(&drawTests);

    renderLoop.run();
}
