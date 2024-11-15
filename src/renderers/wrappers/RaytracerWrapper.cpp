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
    return raytracer.camera;
}

Model& RaytracerWrapper::getModel()
{
    return raytracer.model;
}

Transformation& RaytracerWrapper::getLight()
{
    return raytracer.light;
}
