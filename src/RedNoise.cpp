#include "objects/Model.h"
#include "renderers/RasterRenderer.h"
#include "RenderLoop.h"
#include "handlers/AnimationHandler.h"
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

    auto transformation2 = Transformation();
    transformation2.position = glm::vec3(1, 0, 0);
    auto model2 = Model::import("../models/sphere.obj", materialMap, transformation2);

    auto models = std::vector<Model*>();
    models.push_back(&model);
    models.push_back(&model2);

    auto camera = Transformation(glm::vec3(0, 0, 10),
                         glm::mat3(),
                         1);


    auto keyframe1 = Transformation(glm::vec3(0, 0, 10), glm::mat3(), 1.f);
    auto keyframe2 = Transformation(glm::vec3(10.f/glm::sqrt(2), 0, 10.f/glm::sqrt(2)), glm::mat3(), 1.f);
    auto keyframe3 = Transformation(glm::vec3(10, 0, 0), glm::mat3(), 1.f);
    keyframe3.lookAt(glm::vec3(0, 0, 0));
    keyframe2.lookAt(glm::vec3(0, 0, 0));

    auto cameraAnimator = AnimationHandler(camera);
    cameraAnimator.animation.emplace_back(std::make_pair(1, keyframe1));
    cameraAnimator.animation.emplace_back(std::make_pair(360, keyframe2));
    cameraAnimator.animation.emplace_back(std::make_pair(90, keyframe3));

    auto sphereAnimator = OrbitHandler(transformation2, glm::vec3(0, 0, 0),  true);

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

    rasterTest.preFrameHandlers.push_back(&cameraAnimator);
    rasterTest.preFrameHandlers.push_back(&sphereAnimator);

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
