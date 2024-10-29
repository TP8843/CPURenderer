//
// Created by Thomas Parr on 29/10/2024.
//

#include "WireframeWrapper.h"

WireframeWrapper::WireframeWrapper(RasterRenderer2& renderer) :
    renderer(renderer)
{}

Camera& WireframeWrapper::getCamera()
{
    return renderer.camera;
}

Model& WireframeWrapper::getModel()
{
    return renderer.model;
}

void WireframeWrapper::renderFrame(DrawingWindow& window)
{
    renderer.wireframeRender(window);
}
