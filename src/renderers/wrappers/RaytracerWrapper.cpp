#include "RaytracerWrapper.h"

RaytracerWrapper::RaytracerWrapper(Raytracer& raytracer) :
    raytracer(raytracer)
{}

void RaytracerWrapper::renderFrame(DrawingWindow& window)
{
    raytracer.renderFrame(window);
}

Transformation& RaytracerWrapper::getCamera()
{
    return raytracer.scene.camera;
}

Model& RaytracerWrapper::getModel()
{
    return *raytracer.scene.models.at(0);
}

Transformation& RaytracerWrapper::getLight()
{
    return raytracer.scene.light;
}
