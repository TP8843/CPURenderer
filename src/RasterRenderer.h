#ifndef RASTERRENDERER_H
#define RASTERRENDERER_H

#include <CanvasTriangle.h>
#include <DrawingWindow.h>

#include "helper/Camera.h"
#include "helper/Model.h"


class RasterRenderer
{
public:
    RasterRenderer(Model& model,
                   Camera& camera);

    void pointCloudRender(DrawingWindow& window) const;
    void wireframeRender(DrawingWindow& window) const;
    void rasterRender(DrawingWindow& window) const;

    Model& model;
    Camera& camera;

private:
    CanvasPoint projectVertexOntoCanvasPoint(DrawingWindow& window, glm::vec3 vertexPosition) const;

    static void drawDepthAwareFilledTriangle(DrawingWindow& window,
                                             const CanvasTriangle& triangle,
                                             float** depthBuffer) ;
};


#endif //RASTERRENDERER_H
