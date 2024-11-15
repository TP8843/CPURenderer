//
// Created by Thomas Parr on 29/10/2024.
//

#include "WireframeWrapper.h"

WireframeWrapper::WireframeWrapper(RasterRenderer& renderer) :
    renderer(renderer)
{}

Transformation& WireframeWrapper::getCamera()
{
    return renderer.camera;
}

Model& WireframeWrapper::getModel()
{
    return renderer.model;
}

Transformation& WireframeWrapper::getLight()
{
    auto light = Transformation();
    return light;
}

void WireframeWrapper::renderFrame(DrawingWindow& window)
{
    renderer.wireframeRender(window);
}
