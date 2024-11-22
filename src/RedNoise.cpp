#include "objects/Model.h"
#include "renderers/RasterRenderer.h"
#include "RenderLoop.h"
#include "handlers/CameraControl.h"
#include "handlers/DebugHandler.h"
#include "handlers/OrbitHandler.h"
#include "objects/Scene.h"
#include "renderers/wrappers/RendererWrapper.h"
#include "renderers/wrappers/RasterWrapper.h"
#include "renderers/wrappers/RaytracerWrapper.h"
#include "renderers/wrappers/WireframeWrapper.h"
#include "tests/DrawTests.h"
#include "tests/InteractiveTest.h"
#include "tests/RasterTest.h"

#define WIDTH 640
#define HEIGHT 480

int main(int argc, char* argv[])
{
    std::string modelFile = "../models/textured-cornell-box.obj";
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

    auto materialMap = MaterialMap();

    auto transformation = Transformation();
    transformation.scale = scale;
    auto model = Model::import(modelFile, materialMap, transformation);

    auto models = std::vector<Model*>();
    models.push_back(&model);

    auto camera = Transformation(glm::vec3(0, 0, 10),
                         glm::mat3(),
                         1);

    auto light = Transformation(glm::vec3(0, 2, 0), glm::mat3(), 2.f);

    auto scene = Scene(models, materialMap, camera, light);

    auto orbitHandler = OrbitHandler(camera, glm::vec3());
    auto cameraControl = CameraControl(camera, light);

    auto debugHandler = DebugHandler(model, camera);

    auto renderer = RasterRenderer(scene);
    auto rasterWrapper = RasterWrapper(renderer);
    auto wireframeWrapper = WireframeWrapper(renderer);

    auto raytracer = Raytracer(scene);
    auto raytracerWrapper = RaytracerWrapper(raytracer);

    auto rasterTest = RasterTest();
    rasterTest.addRendererWrapper(&rasterWrapper);
    rasterTest.addRendererWrapper(&wireframeWrapper);
    rasterTest.addRendererWrapper(&raytracerWrapper);

    rasterTest.preFrameHandlers.push_back(&cameraControl);
    rasterTest.eventHandlers.push_back(&cameraControl);

    rasterTest.preFrameHandlers.push_back(&orbitHandler);
    rasterTest.eventHandlers.push_back(&orbitHandler);

    rasterTest.postFrameHandlers.push_back(&debugHandler);
    rasterTest.eventHandlers.push_back(&debugHandler);

    renderLoop.addTest(&rasterTest);

    auto interactiveTest = InteractiveTest();
    renderLoop.addTest(&interactiveTest);


    auto drawTests = DrawTests();
    renderLoop.addTest(&drawTests);

    renderLoop.run();
}
