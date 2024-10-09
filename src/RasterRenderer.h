//
// Created by Thomas Parr on 09/10/2024.
//

#ifndef RASTERRENDERER_H
#define RASTERRENDERER_H
#include <DrawingWindow.h>

#include "Model.h"


class RasterRenderer
{
public:
    RasterRenderer(Model& model,
                   glm::vec3 cameraPosition,
                   float focalLength,
                   float imagePlaneScaling);

    void pointCloudRender(DrawingWindow &window) const;
    void wireframeRender(DrawingWindow &window) const;
    void rasterRender(DrawingWindow &window) const;

    glm::vec3 cameraPosition;
    float focalLength;
    float imagePlaneScaling;
    Model& model;

private:
    CanvasPoint projectVertexOntoCanvasPoint(DrawingWindow &window, glm::vec3 vertexPosition) const;
};


#endif //RASTERRENDERER_H
