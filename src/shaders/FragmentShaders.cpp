//
// Created by Thomas Parr on 28/10/2024.
//

#include "FragmentShaders.h"

void FragmentShaders::filled(DrawingWindow& window, CanvasTriangle triangle, const int x, const int y,
    float** depthBuffer, const PointData::FilledData& data)
{
    if (data.depth > 0 && data.depth < 0.7 && x >= 0 && x < window.width && y >= 0 && y < window.height &&
    data.depth > depthBuffer[y][x])
    {
        depthBuffer[y][x] = data.depth;

        window.setPixelColour(x, y, data.colour.asARGB());
    }
}

void FragmentShaders::rainbow(DrawingWindow& window, CanvasTriangle triangle, int x, int y, float** depthBuffer,
    const PointData::FilledData& data)
{
    if (data.depth > 0 && data.depth < 0.7 && x >= 0 && x < window.width && y >= 0 && y < window.height && data.depth > depthBuffer[y][x])
    {
        depthBuffer[y][x] = data.depth;

        window.setPixelColour(x, y, Colour(data.proportion[0] * 255, data.proportion[1] * 255, data.proportion[2] * 255).asARGB());
    }
}

void FragmentShaders::outline(DrawingWindow& window, CanvasTriangle triangle, int x, int y, float** depthBuffer,
    const PointData::FilledData& data)
{
    if (data.depth > 0 && data.depth < 0.7 && x >= 0 && x < window.width && y >= 0 && y < window.height && data.depth > depthBuffer[y][x]
        && glm::min(data.proportion[0], glm::min(data.proportion[1], data.proportion[2])) < 0.05
)
    {
        depthBuffer[y][x] = data.depth;

        window.setPixelColour(x, y, data.colour.asARGB());
    }
}

void FragmentShaders::depth(DrawingWindow& window, CanvasTriangle triangle, const int x, const int y,
                            float** depthBuffer, const PointData::FilledData& data)
{
    if (data.depth > 0 && data.depth < 0.7 && x >= 0 && x < window.width && y >= 0 && y < window.height &&
    data.depth > depthBuffer[y][x])
    {
        auto multiplier = glm::clamp(data.depth, 0.0f, 1.0f) * 255 * 3;

        depthBuffer[y][x] = data.depth;

        window.setPixelColour(x, y, Colour(multiplier, multiplier, multiplier).asARGB());
    }
}
