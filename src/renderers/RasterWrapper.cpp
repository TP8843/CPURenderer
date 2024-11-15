#include "RasterWrapper.h"

RasterWrapper::RasterWrapper(RasterRenderer& renderer) :
    renderer(renderer)
{}

void RasterWrapper::renderFrame(DrawingWindow& window)
{
    renderer.rasterRender(window);
}

Transformation& RasterWrapper::getCamera()
{
    return renderer.camera;
}

Model& RasterWrapper::getModel()
{
    return renderer.model;
}

Transformation& RasterWrapper::getLight()
{
    return renderer.light;
}
