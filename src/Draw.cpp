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
    const auto colourValue = (255 << 24) + (colour.red << 16) + (colour.green << 8) + colour.blue;
    std::array<CanvasPoint, 3> vertices =  triangle.vertices;

    // Sort triangle based on y value
    if (vertices[0].y > vertices[1].y) std::swap(vertices[0], vertices[1]);
    if (vertices[1].y > vertices[2].y) std::swap(vertices[1], vertices[2]);
    if (vertices[0].y > vertices[1].y) std::swap(vertices[0], vertices[1]);

    int startVertex = 0;

    for(size_t y = glm::floor(vertices[0].y); y <= static_cast<size_t>(glm::floor(vertices[2].y)); y++)
    {
        const float rowStartProportion = (static_cast<float>(y) - vertices[startVertex].y) / (vertices[startVertex + 1].y - vertices[startVertex].y);
        float rowStart = Interpolation::interpolateSingleFloat(vertices[startVertex].x, vertices[startVertex + 1].x, rowStartProportion);

        const float rowEndProportion = (static_cast<float>(y) - vertices[0].y) / (vertices[2].y - vertices[0].y);
        float rowEnd = Interpolation::interpolateSingleFloat(vertices[0].x, vertices[2].x, rowEndProportion);

        if (rowStart > rowEnd) std::swap(rowStart, rowEnd);

        for(size_t x = glm::ceil(rowStart); x < static_cast<size_t>(glm::ceil(rowEnd)); x++)
        {
            window.setPixelColour(x, y, colourValue);
        }

        if (y == static_cast<size_t>(vertices[1].y)) startVertex++;
    }
}

void Draw::drawTexturedTriangle(DrawingWindow& window, const CanvasTriangle& triangle, const TextureMap& texture)
{
    auto vertices =  triangle.vertices;

    // Sort triangle based on y value
    if (vertices[0].y > vertices[1].y) std::swap(vertices[0], vertices[1]);
    if (vertices[1].y > vertices[2].y) std::swap(vertices[1], vertices[2]);
    if (vertices[0].y > vertices[1].y) std::swap(vertices[0], vertices[1]);

    int startVertex = 0;

    for(size_t y = glm::floor(vertices[0].y); y <= static_cast<size_t>(glm::floor(vertices[2].y)); y++)
    {
        const float rowStartProportion = (static_cast<float>(y) - vertices[startVertex].y) / (vertices[startVertex + 1].y - vertices[startVertex].y);
        float rowStart = Interpolation::interpolateSingleFloat(vertices[startVertex].x, vertices[startVertex + 1].x, rowStartProportion);
        auto textureRowStart = TexturePoint(
            Interpolation::interpolateSingleFloat(vertices[startVertex].texturePoint.x, vertices[startVertex + 1].texturePoint.x, rowStartProportion),
            Interpolation::interpolateSingleFloat(vertices[startVertex].texturePoint.y, vertices[startVertex + 1].texturePoint.y, rowStartProportion)
            );

        const float rowEndProportion = (static_cast<float>(y) - vertices[0].y) / (vertices[2].y - vertices[0].y);
        float rowEnd = Interpolation::interpolateSingleFloat(vertices[0].x, vertices[2].x, rowEndProportion);
        auto textureRowEnd = TexturePoint(
            Interpolation::interpolateSingleFloat(vertices[0].texturePoint.x, vertices[2].texturePoint.x, rowEndProportion),
            Interpolation::interpolateSingleFloat(vertices[0].texturePoint.y, vertices[2].texturePoint.y, rowEndProportion)
            );

        if (rowStart > rowEnd)
        {
            std::swap(rowStart, rowEnd);
            std::swap(textureRowStart, textureRowEnd);
        }

        for(size_t x = glm::ceil(rowStart); x < static_cast<size_t>(glm::ceil(rowEnd)); x++)
        {
            const float currentRowProportion = (static_cast<float>(x) - rowStart) / (rowEnd - rowStart);

            const auto texturePosition = TexturePoint(
                Interpolation::interpolateSingleFloat(textureRowStart.x, textureRowEnd.x, currentRowProportion),
                Interpolation::interpolateSingleFloat(textureRowStart.y, textureRowEnd.y, currentRowProportion)
            );
            const size_t colour = texture.pixels.at(glm::floor(texturePosition.y) * static_cast<float>(texture.width) + glm::floor(texturePosition.x));
            const auto colourValue = (0xFF << 24) + (colour & 0xFFFFFF);
            window.setPixelColour(x, y, colourValue);
        }

        if (y == static_cast<size_t>(vertices[1].y)) startVertex++;
    }

    // const auto sliceEndTexturePoint = TexturePoint(Interpolation::interpolateSingleFloat(vMin.texturePoint.x, vMax.texturePoint.x, textureProportion),
    //                                                Interpolation::interpolateSingleFloat(vMin.texturePoint.y, vMax.texturePoint.y, textureProportion));
}

void Draw::drawTexturedFlatBottomTriangle(DrawingWindow &window, const CanvasPoint &sliceStart, const CanvasPoint &sliceEnd, const CanvasPoint &other, const TextureMap &texture)
{

    for (int y = glm::floor(sliceStart.y); y < static_cast<int>(glm::floor(other.y)); y++)
    {
        const float yProportion = (static_cast<float>(y) - sliceStart.y) / (other.y - sliceStart.y);

        float start = Interpolation::interpolateSingleFloat(sliceStart.x, other.x, yProportion);
        float end = Interpolation::interpolateSingleFloat(sliceEnd.x, other.x, yProportion);

        auto textureStart = TexturePoint(
            Interpolation::interpolateSingleFloat(sliceStart.texturePoint.x, other.texturePoint.x, yProportion),
            Interpolation::interpolateSingleFloat(sliceStart.texturePoint.y, other.texturePoint.y, yProportion)
        );

        auto textureEnd = TexturePoint(
            Interpolation::interpolateSingleFloat(sliceEnd.texturePoint.x, other.texturePoint.x, yProportion),
            Interpolation::interpolateSingleFloat(sliceEnd.texturePoint.y, other.texturePoint.y, yProportion)
        );

        if (start > end)
        {
            std::swap(start, end);
            std::swap(textureStart, textureEnd);
        }

        for (int x = glm::ceil(start); x < static_cast<int>(glm::ceil(end)); x++)
        {
            const float xProportion = (static_cast<float>(x) - start) / (end - start);
            const auto texturePoint = TexturePoint(
                Interpolation::interpolateSingleFloat(textureStart.x, textureEnd.x, xProportion),
                Interpolation::interpolateSingleFloat(textureStart.y, textureEnd.y, xProportion)
            );

            const int textureColour = texture.pixels.at(glm::round(texturePoint.y) * texture.width + glm::round(texturePoint.x));
            const int formattedColour = (0xFF << 24) + (textureColour & 0xFFFFFF);

            window.setPixelColour(x, y, formattedColour);
        }
    }
}

void Draw::drawTexturedFlatTopTriangle(DrawingWindow &window, const CanvasPoint &sliceStart, const CanvasPoint &sliceEnd, const CanvasPoint &other, const TextureMap &texture)
{

    for (int y = glm::floor(sliceStart.y); y > static_cast<int>(glm::floor(other.y)); y--)
    {
        const float yProportion = (static_cast<float>(y) - sliceStart.y) / (other.y - sliceStart.y);
        const float textureY = Interpolation::interpolateSingleFloat(sliceStart.texturePoint.y, other.texturePoint.y, yProportion);

        float start = Interpolation::interpolateSingleFloat(sliceStart.x, other.x, yProportion);
        float end = Interpolation::interpolateSingleFloat(sliceEnd.x, other.x, yProportion);
        float textureStart = Interpolation::interpolateSingleFloat(sliceStart.texturePoint.x, other.texturePoint.x, yProportion);
        float textureEnd = Interpolation::interpolateSingleFloat(sliceEnd.texturePoint.x, other.texturePoint.x, yProportion);

        if (start > end)
        {
            std::swap(start, end);
            std::swap(textureStart, textureEnd);
        }

        for (int x = glm::ceil(start); x < static_cast<int>(glm::ceil(end)); x++)
        {
            const float xProportion = (static_cast<float>(x) - start) / (end - start);
            const float textureX = Interpolation::interpolateSingleFloat(textureStart, textureEnd, xProportion);

            const int textureColour = texture.pixels.at(glm::round(textureY) * texture.width + glm::round(textureX));
            const int formattedColour = (0xFF << 24) + (textureColour & 0xFFFFFF);

            window.setPixelColour(x, y, formattedColour);
        }
    }
}