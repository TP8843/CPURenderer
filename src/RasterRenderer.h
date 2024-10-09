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
    RasterRenderer(DrawingWindow& window, Model& model, glm::vec3 cameraPosition, float focalLength,
                   float imagePlaneScaling);

    void pointCloudRender() const;
    void wireframeRender() const;
    void rasterRender() const;

    glm::vec3 cameraPosition;
    float focalLength;
    float imagePlaneScaling;
    Model& model;

private:
    CanvasPoint projectVertexOntoCanvasPoint(glm::vec3 vertexPosition) const;

    DrawingWindow& window;
};


#endif //RASTERRENDERER_H
