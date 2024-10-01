#include "Draw.h"

#include <CanvasPoint.h>
#include <Colour.h>

#include "Interpolation.h"

void Draw::drawLine(DrawingWindow &window, const CanvasPoint &from, const CanvasPoint &to, const Colour &colour)
{
    const auto yDif = glm::abs(to.y - from.y);
    const auto xDif = glm::abs(to.x - from.x);

    const auto maxDif = glm::max(yDif, xDif);

    std::vector<float> xs = Interpolation::interpolateSingleFloats(from.x, to.x, maxDif + 1);
    std::vector<float> ys = Interpolation::interpolateSingleFloats(from.y, to.y, maxDif + 1);
    const auto colourValue = (255 << 24) + (colour.red << 16) + (colour.green << 8) + colour.blue;

    // Run algorithm row by row
    for (int i = 0; i < maxDif; i++)
    {
        int x = xs[i];
        int y = ys[i];

        window.setPixelColour(x, y, colourValue);
    }

}

void Draw::drawStrokedTriangle(DrawingWindow& window, const CanvasTriangle& triangle, const Colour& colour)
{
    const auto v1 = triangle.vertices[0];
    const auto v2 = triangle.vertices[1];
    const auto v3 = triangle.vertices[2];

    drawLine(window, v1, v2, colour);
    drawLine(window, v1, v3, colour);
    drawLine(window, v2, v3, colour);
}

void Draw::drawFilledTriangle(DrawingWindow &window, const CanvasTriangle& triangle, const Colour &colour)
{
    auto vMin = triangle.vertices[0];
    auto vMid = triangle.vertices[1];
    auto vMax = triangle.vertices[2];

    // Sort triangle based on y value
    if (vMin.y > vMid.y) std::swap(vMin, vMid);
    if (vMid.y > vMax.y) std::swap(vMid, vMax);
    if (vMin.y > vMid.y) std::swap(vMin, vMid);

    const float proportion = (vMid.y - vMin.y) / (vMax.y - vMin.y);

    const auto sliceStart = vMid;
    const auto sliceEnd = CanvasPoint(Interpolation::interpolateSingleFloat(vMin.x, vMax.x, proportion), vMid.y);

    drawFlatBottomTriangle(window, sliceStart, sliceEnd, vMax, colour);
    drawFlatTopTriangle(window, sliceStart, sliceEnd, vMin, colour);
}

 void Draw::drawFlatBottomTriangle(DrawingWindow &window, const CanvasPoint &sliceStart,
     const CanvasPoint &sliceEnd, const CanvasPoint &other, const Colour &colour)
{
    const auto colourValue = (255 << 24) + (colour.red << 16) + (colour.green << 8) + colour.blue;

    for (int y = sliceStart.y; y < other.y; y++)
    {
        float proportion = (y - sliceStart.y) / (other.y - sliceStart.y);
        int start = Interpolation::interpolateSingleFloat(sliceStart.x, other.x, proportion);
        int end = Interpolation::interpolateSingleFloat(sliceEnd.x, other.x, proportion);

        if (start > end) std::swap(start, end);

        for (int x = start; x < end; x++)
        {
            window.setPixelColour(x, y, colourValue);
        }
    }
}

void Draw::drawFlatTopTriangle(DrawingWindow &window, const CanvasPoint &sliceStart,
    const CanvasPoint &sliceEnd, const CanvasPoint &other, const Colour &colour)
{
    const auto colourValue = (255 << 24) + (colour.red << 16) + (colour.green << 8) + colour.blue;

    for (int y = other.y; y < sliceStart.y; y++)
    {
        float proportion = (y - other.y) / (sliceStart.y - other.y);
        int start = Interpolation::interpolateSingleFloat(other.x, sliceStart.x, proportion);
        int end = Interpolation::interpolateSingleFloat(other.x, sliceEnd.x, proportion);

        if (start > end) std::swap(start, end);

        for (int x = start; x < end; x++)
        {
            window.setPixelColour(x, y, colourValue);
        }
    }
}