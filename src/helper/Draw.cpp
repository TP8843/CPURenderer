#include "Draw.h"

#include <CanvasPoint.h>
#include <Colour.h>
#include <TextureMap.h>

#include "Interpolation.h"

void Draw::drawLine(DrawingWindow& window, const CanvasPoint& from, const CanvasPoint& to, const Colour& colour)
{
    const auto yDif = glm::abs(to.y - from.y);
    const auto xDif = glm::abs(to.x - from.x);
    const auto colourValue = colour.asARGB();

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

        for (int x = static_cast<int>(glm::floor(start.x)); x <= static_cast<int>(glm::floor(end.x)); x++)
        {
            const float proportion = (static_cast<float>(x) - start.x) / (end.x - start.x);

            const int y = static_cast<int>(glm::floor(Interpolation::interpolate(start.y, end.y, proportion)));

            if (x >= 0 && y >= 0 && x < static_cast<int>(window.width) && y < static_cast<int>(window.height))
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

        for (int y = static_cast<int>(glm::floor(start.y)); y <= static_cast<int>(glm::floor(end.y)); y++)
        {
            const float proportion = (static_cast<float>(y) - start.y) / (end.y - start.y);

            const int x = static_cast<int>(glm::floor(Interpolation::interpolate(start.x, end.x, proportion)));

            if (x >= 0 && y >= 0 && x < static_cast<int>(window.width) && y < static_cast<int>(window.height))
                window.setPixelColour(x, y, colourValue);
        }
    }
}

void Draw::drawStrokedTriangle(DrawingWindow& window, const CanvasTriangle& triangle)
{
    const auto v1 = triangle.vertices[0];
    const auto v2 = triangle.vertices[1];
    const auto v3 = triangle.vertices[2];

    drawLine(window, v1, v2, triangle.colour);
    drawLine(window, v1, v3, triangle.colour);
    drawLine(window, v2, v3, triangle.colour);
}

void Draw::drawFilledTriangle(DrawingWindow& window, const CanvasTriangle& triangle)
{
    const auto colourValue = triangle.colour.asARGB();
    std::array<CanvasPoint, 3> vertices = triangle.vertices;

    // Sort triangle based on y value
    if (vertices[0].y > vertices[1].y) std::swap(vertices[0], vertices[1]);
    if (vertices[1].y > vertices[2].y) std::swap(vertices[1], vertices[2]);
    if (vertices[0].y > vertices[1].y) std::swap(vertices[0], vertices[1]);

    int startVertex = 0;

    for (auto y = static_cast<size_t>(glm::floor(vertices[0].y)); y <= static_cast<size_t>(glm::floor(vertices[2].y)); y
         ++)
    {
        const float rowStartProportion = Interpolation::proportion(vertices[startVertex].y,
                                                                   vertices[startVertex + 1].y,
                                                                   static_cast<float>(y), 0);

        float rowStart = Interpolation::interpolate(vertices[startVertex].x,
                                                    vertices[startVertex + 1].x,
                                                    rowStartProportion);

        const float rowEndProportion = Interpolation::proportion(vertices[0].y, vertices[2].y,
                                                                 static_cast<float>(y), 1);

        float rowEnd = Interpolation::interpolate(vertices[0].x, vertices[2].x, rowEndProportion);

        if (rowStart > rowEnd) std::swap(rowStart, rowEnd);

        for (auto x = static_cast<size_t>(glm::ceil(rowStart)); x < static_cast<size_t>(glm::ceil(rowEnd)); x++)
        {
            window.setPixelColour(x, y, colourValue);
        }

        if (y == static_cast<size_t>(vertices[1].y)) startVertex++;
    }
}

void Draw::drawTexturedTriangle(DrawingWindow& window, const CanvasTriangle& triangle, const TextureMap& texture)
{
    auto vertices = triangle.vertices;

    // Sort triangle based on y value
    if (vertices[0].y > vertices[1].y) std::swap(vertices[0], vertices[1]);
    if (vertices[1].y > vertices[2].y) std::swap(vertices[1], vertices[2]);
    if (vertices[0].y > vertices[1].y) std::swap(vertices[0], vertices[1]);

    int startVertex = 0;

    for (auto y = static_cast<size_t>(glm::floor(vertices[0].y)); y <= static_cast<size_t>(glm::floor(vertices[2].y)); y
         ++)
    {
        const float rowStartProportion = Interpolation::proportion(vertices[startVertex].y, vertices[startVertex + 1].y,
                                                                   static_cast<float>(y), 0);

        float rowStart = Interpolation::interpolate(vertices[startVertex].x, vertices[startVertex + 1].x,
                                                    rowStartProportion);
        auto textureRowStart = TexturePoint(
            Interpolation::interpolate(vertices[startVertex].texturePoint.x, vertices[startVertex + 1].texturePoint.x,
                                       rowStartProportion),
            Interpolation::interpolate(vertices[startVertex].texturePoint.y, vertices[startVertex + 1].texturePoint.y,
                                       rowStartProportion)
        );

        const float rowEndProportion = Interpolation::proportion(vertices[0].y, vertices[2].y,
                                                                 static_cast<float>(y), 1);

        float rowEnd = Interpolation::interpolate(vertices[0].x, vertices[2].x, rowEndProportion);
        auto textureRowEnd = TexturePoint(
            Interpolation::interpolate(vertices[0].texturePoint.x, vertices[2].texturePoint.x, rowEndProportion),
            Interpolation::interpolate(vertices[0].texturePoint.y, vertices[2].texturePoint.y, rowEndProportion)
        );

        if (rowStart > rowEnd)
        {
            std::swap(rowStart, rowEnd);
            std::swap(textureRowStart, textureRowEnd);
        }

        for (auto x = static_cast<size_t>(glm::ceil(rowStart)); x < static_cast<size_t>(glm::ceil(rowEnd)); x++)
        {
            const float currentRowProportion = Interpolation::proportion(rowStart, rowEnd,
                                                                         static_cast<float>(x), 0);

            const auto texturePosition = TexturePoint(
                Interpolation::interpolate(textureRowStart.x, textureRowEnd.x, currentRowProportion),
                Interpolation::interpolate(textureRowStart.y, textureRowEnd.y, currentRowProportion)
            );

            const size_t colour = texture.pixels.at(
                static_cast<int>(glm::floor(texturePosition.y) * static_cast<float>(texture.width) + glm::floor(texturePosition.x)));

            const auto colourValue = (0xFF << 24) + (colour & 0xFFFFFF);
            window.setPixelColour(x, y, colourValue);
        }

        if (y == static_cast<size_t>(vertices[1].y)) startVertex++;
    }
}
