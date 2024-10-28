//
// Created by Thomas Parr on 28/10/2024.
//

#ifndef RASTERRENDERER2_H
#define RASTERRENDERER2_H
#include <CanvasTriangle.h>

#include "helper/Camera.h"
#include "helper/Interpolation.h"
#include "helper/Model.h"

// Electric Boogaloo
class RasterRenderer2
{
public:
    RasterRenderer2(Model& model, Camera& camera);

    void pointCloudRender(DrawingWindow& window) const;
    void wireframeRender(DrawingWindow& window) const;
    void rasterRender(DrawingWindow& window) const;

    Model& model;
    Camera& camera;

private:
    static float** createDepthBuffer(int width, int height);
    CanvasPoint projectVertexOntoCanvasPoint(glm::vec3 vertex, int width, int height) const;

    template <typename Data>
    static void drawTriangle(DrawingWindow& window,
                             CanvasTriangle triangle,
                             float** depthBuffer,
                             std::array<Data, 3> verticesData,
                             std::function<void (DrawingWindow&,
                                                 CanvasTriangle,
                                                 int,
                                                 int,
                                                 float**,
                                                 const Data&)> fragmentShader)
    {
        auto vertices = std::array<CanvasPoint, 3>(triangle.vertices);
        auto fragmentData = std::array<Data, 3>(verticesData);

        // Sort triangles based on y value
        if (vertices[0].y > vertices[1].y)
        {
            std::swap(vertices[0], vertices[1]);
            std::swap(fragmentData[0], fragmentData[1]);
        }

        if (vertices[1].y > vertices[2].y)
        {
            std::swap(vertices[1], vertices[2]);
            std::swap(fragmentData[1], fragmentData[2]);
        }

        if (vertices[0].y > vertices[1].y)
        {
            std::swap(vertices[0], vertices[1]);
            std::swap(fragmentData[0], fragmentData[1]);
        }

        int startVertex = 0;
        for (int y = glm::floor(vertices[0].y); y <= static_cast<int>(glm::floor(vertices[2].y)); y++)
        {
            const float rowStartProportion =
                Interpolation::proportion(vertices[startVertex].y,
                                          vertices[startVertex + 1].y,
                                          y, 0);

            const float rowEndProportion =
                Interpolation::proportion(vertices[0].y,
                                          vertices[2].y,
                                          y, 1);

            float rowStartX =
                Interpolation::interpolate(vertices[startVertex].x,
                                           vertices[startVertex + 1].x,
                                           rowStartProportion);

            Data rowStartData =
                Interpolation::interpolate(fragmentData[startVertex],
                                           fragmentData[startVertex + 1],
                                           rowStartProportion);

            float rowEndX =
                Interpolation::interpolate(vertices[0].x,
                                           vertices[2].x,
                                           rowEndProportion);

            Data rowEndData =
                Interpolation::interpolate(fragmentData[0],
                                           fragmentData[2],
                                           rowEndProportion);

            if (rowStartX > rowEndX)
            {
                std::swap(rowStartX, rowEndX);
                std::swap(rowStartData, rowEndData);
            }

            for (int x = glm::ceil(rowStartX); x < static_cast<int>(glm::ceil(rowEndX)); x++)
            {
                const float proportion = Interpolation::proportion(rowStartX, rowEndX, x, 0);
                const Data data = Interpolation::interpolate(rowStartData, rowEndData, proportion);

                fragmentShader(window, triangle, x, y, depthBuffer, data);
            }

            if (y == static_cast<int>(vertices[1].y)) startVertex++;
        }
    }
};


#endif //RASTERRENDERER2_H
