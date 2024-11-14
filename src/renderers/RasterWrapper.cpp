#include "RasterWrapper.h"

RasterWrapper::RasterWrapper(RasterRenderer& renderer) :
    renderer(renderer)
{}

void RasterWrapper::renderFrame(DrawingWindow& window)
{
    renderer.rasterRender(window);
}

Camera& RasterWrapper::getCamera()
{
    return renderer.camera;
}

Model& RasterWrapper::getModel()
{
    return renderer.model;
}

Light& RasterWrapper::getLight()
{
    return renderer.light;
}
