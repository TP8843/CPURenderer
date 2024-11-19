//
// Created by Thomas Parr on 29/10/2024.
//

#include "WireframeWrapper.h"

WireframeWrapper::WireframeWrapper(RasterRenderer& renderer) :
    renderer(renderer)
{}

Transformation& WireframeWrapper::getCamera()
{
    return renderer.scene.camera;
}

Model& WireframeWrapper::getModel()
{
    return *renderer.scene.models.at(0);
}

Transformation& WireframeWrapper::getLight()
{
    return renderer.scene.light;
}

void WireframeWrapper::renderFrame(DrawingWindow& window)
{
    renderer.wireframeRender(window);
}
