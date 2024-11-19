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
    return renderer.scene.camera;
}

Model& RasterWrapper::getModel()
{
    return *renderer.scene.models.at(0);
}

Transformation& RasterWrapper::getLight()
{
    return renderer.scene.light;
}
