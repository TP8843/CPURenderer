//
// Created by Thomas Parr on 29/10/2024.
//

#include "RasterWrapper.h"

RasterWrapper::RasterWrapper(RasterRenderer2& renderer) :
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
