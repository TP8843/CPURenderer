#ifndef RASTERRENDERER2_H
#define RASTERRENDERER2_H
#include <functional>
#include <CanvasTriangle.h>
#include <DrawingWindow.h>

#include "../helper/Interpolation.h"
#include "../objects/Model.h"

// Electric Boogaloo
class RasterRenderer
{
public:
    RasterRenderer(Model& model, Transformation& camera, Transformation& light);

    void pointCloudRender(DrawingWindow& window) const;
    void wireframeRender(DrawingWindow& window) const;
    void rasterRender(DrawingWindow& window) const;

    Model& model;
    Transformation& camera;
    Transformation& light;

private:
    float** generateDepthBuffer(const std::vector<ModelTriangle>& triangles, size_t width, size_t height) const;
    glm::vec3 applyCameraTransformation(glm::vec3 vertex) const;
    CanvasPoint projectVertexOntoCanvasPoint(glm::vec3 vertex, size_t width, size_t height) const;

    template <typename Uniform, typename Data>
    static void drawTriangle(CanvasTriangle triangle,
                             Uniform uniform,
                             std::array<Data, 3> fragmentData,
                             std::function<void (const CanvasTriangle&,
                                                 int,
                                                 int,
                                                 const Uniform&,
                                                 const Data&)> fragmentShader)
    {
        auto vertices = triangle.vertices;
        // auto fragmentData = std::array<Data, 3>(verticesData);

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
        for (int y = static_cast<int>(glm::floor(vertices[0].y)); y <= static_cast<int>(glm::floor(vertices[2].y)); y++)
        {
            const float rowStartProportion =
                Interpolation::proportion(vertices[startVertex].y,
                                          vertices[startVertex + 1].y,
                                          static_cast<float>(y), 0);

            const float rowEndProportion =
                Interpolation::proportion(vertices[0].y,
                                          vertices[2].y,
                                          static_cast<float>(y), 1);

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

            for (int x = static_cast<int>(glm::ceil(rowStartX)); x < static_cast<int>(glm::ceil(rowEndX)); x++)
            {
                const float proportion = Interpolation::proportion(rowStartX, rowEndX, static_cast<float>(x), 0);
                const Data data = Interpolation::interpolate(rowStartData, rowEndData, proportion);

                fragmentShader(triangle, x, y, uniform, data);
            }

            if (y == static_cast<int>(vertices[1].y)) startVertex++;
        }
    }
};


#endif //RASTERRENDERER2_H
