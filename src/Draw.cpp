#include "Draw.h"

#include <CanvasPoint.h>
#include <Colour.h>
#include <TextureMap.h>

#include "Interpolation.h"

void Draw::drawLine(DrawingWindow& window, const CanvasPoint& from, const CanvasPoint& to, const Colour& colour)
{
    const auto yDif = glm::abs(to.y - from.y);
    const auto xDif = glm::abs(to.x - from.x);
    const auto colourValue = (255 << 24) + (colour.red << 16) + (colour.green << 8) + colour.blue;

    if (xDif >= yDif)
    {
        // Use x as a base
        CanvasPoint start;
        CanvasPoint end;

        if (from.x > to.x)
        {
            start = to;
            end = from;
        }
        else
        {
            start = from;
            end = to;
        }

        for (int x = glm::floor(start.x); x <= static_cast<int>(glm::floor(end.x)); x++)
        {
            const float proportion = (static_cast<float>(x) - start.x) / (end.x - start.x);

            const int y = static_cast<int>(glm::floor(Interpolation::interpolateSingleFloat(start.y, end.y, proportion)));
            window.setPixelColour(x, y, colourValue);
        }
    }
    else
    {
        // Use y as a base
        CanvasPoint start;
        CanvasPoint end;

        if (from.y > to.y)
        {
            start = to;
            end = from;
        }
        else
        {
            start = from;
            end = to;
        }

        for (int y = glm::floor(start.y); y <= static_cast<int>(glm::floor(end.y)); y++)
        {
            const float proportion = (static_cast<float>(y) - start.y) / (end.y - start.y);

            const int x = static_cast<int>(glm::floor(Interpolation::interpolateSingleFloat(start.x, end.x, proportion)));
            window.setPixelColour(x, y, colourValue);
        }
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

void Draw::drawFilledTriangle(DrawingWindow& window, const CanvasTriangle& triangle, const Colour &colour)
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

    drawFilledFlatBottomTriangle(window, sliceStart, sliceEnd, vMax, colour);
    drawFilledFlatTopTriangle(window, sliceStart, sliceEnd, vMin, colour);
}

void Draw::drawTexturedTriangle(DrawingWindow& window, const CanvasTriangle& triangle, const TextureMap& texture)
{
    auto vMin = triangle.vertices[0];
    auto vMid = triangle.vertices[1];
    auto vMax = triangle.vertices[2];

    // Sort triangle based on y value
    if (vMin.y > vMid.y) std::swap(vMin, vMid);
    if (vMid.y > vMax.y) std::swap(vMid, vMax);
    if (vMin.y > vMid.y) std::swap(vMin, vMid);

    const float canvasProportion = (vMid.y - vMin.y) / (vMax.y - vMin.y);
    const float textureXProportion = (vMid.texturePoint.x - vMin.texturePoint.x) / (vMax.texturePoint.x - vMin.texturePoint.x);
    const float textureYProportion = (vMid.texturePoint.y - vMin.texturePoint.y) / (vMax.texturePoint.y - vMin.texturePoint.y);

    const auto sliceStart = vMid;
    const auto sliceEnd = CanvasPoint(Interpolation::interpolateSingleFloat(vMin.x, vMax.x, canvasProportion), vMid.y, );
    sliceEnd.texturePoint = TexturePoint(Interpolation::interpolateSingleFloat(vMin.texturePoint.x, vMax.texturePoint.x, textureXProportion),
                                         Interpolation::interpolateSingleFloat(vMin.texturePoint.y, vMax.texturePoint.y, textureYProportion));
}

 void Draw::drawFilledFlatBottomTriangle(DrawingWindow &window, const CanvasPoint &sliceStart,
     const CanvasPoint &sliceEnd, const CanvasPoint &other, const Colour &colour)
{
    const auto colourValue = (255 << 24) + (colour.red << 16) + (colour.green << 8) + colour.blue;

    for (int y = glm::floor(sliceStart.y); y < static_cast<int>(glm::floor(other.y)); y++)
    {
        const float proportion = (static_cast<float>(y) - sliceStart.y) / (other.y - sliceStart.y);
        float start = Interpolation::interpolateSingleFloat(sliceStart.x, other.x, proportion);
        float end = Interpolation::interpolateSingleFloat(sliceEnd.x, other.x, proportion);

        if (start > end) std::swap(start, end);

        for (int x = glm::ceil(start); x < static_cast<int>(glm::ceil(end)); x++)
        {
            window.setPixelColour(x, y, colourValue);
        }
    }
}

void Draw::drawFilledFlatTopTriangle(DrawingWindow &window, const CanvasPoint &sliceStart,
    const CanvasPoint &sliceEnd, const CanvasPoint &other, const Colour &colour)
{
    const auto colourValue = (255 << 24) + (colour.red << 16) + (colour.green << 8) + colour.blue;

    for (int y = glm::floor(sliceStart.y); y > static_cast<int>(glm::floor(other.y)); y--)
    {
        const float proportion = (static_cast<float>(y) - sliceStart.y) / (other.y - sliceStart.y);
        float start = Interpolation::interpolateSingleFloat(sliceStart.x, other.x, proportion);
        float end = Interpolation::interpolateSingleFloat(sliceEnd.x, other.x, proportion);

        if (start > end) std::swap(start, end);

        for (int x = glm::ceil(start); x < static_cast<int>(glm::ceil(end)); x++)
        {
            window.setPixelColour(x, y, colourValue);
        }
    }
}