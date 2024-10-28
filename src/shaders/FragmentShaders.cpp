//
// Created by Thomas Parr on 28/10/2024.
//

#include "FragmentShaders.h"

void FragmentShaders::filled(CanvasTriangle triangle, int x, int y,
                             const FragmentData::FilledDataUniform& uniform, const FragmentData::FilledData& data)
{
    if (data.depth > 0 && data.depth < 0.7 &&
        x >= 0 && x < uniform.window.width &&
        y >= 0 && y < uniform.window.height &&
        data.depth > uniform.depthBuffer[y][x])
    {
        uniform.depthBuffer[y][x] = data.depth;

        uniform.window.setPixelColour(x, y, data.colour.asARGB());
    }
}

void FragmentShaders::rainbow(CanvasTriangle triangle, const int x, const int y,
                              const FragmentData::FilledDataUniform& uniform,
                              const FragmentData::FilledData& data)
{
    if (data.depth > 0 && data.depth < 0.7 &&
        x >= 0 && x < uniform.window.width &&
        y >= 0 && y < uniform.window.height &&
        data.depth > uniform.depthBuffer[y][x])
    {
        uniform.depthBuffer[y][x] = data.depth;

        uniform.window.setPixelColour(
            x, y, Colour(data.proportion[0] * 255, data.proportion[1] * 255, data.proportion[2] * 255).asARGB());
    }
}

void FragmentShaders::outline(CanvasTriangle triangle, const int x, const int y,
                              const FragmentData::FilledDataUniform& uniform,
                              const FragmentData::FilledData& data)
{
    if (data.depth > 0 && data.depth < 0.7 &&
        x >= 0 && x < uniform.window.width &&
        y >= 0 && y < uniform.window.height &&
        data.depth > uniform.depthBuffer[y][x]
        && glm::min(data.proportion[0], glm::min(data.proportion[1], data.proportion[2])) < 0.5 * data.depth
    )
    {
        uniform.depthBuffer[y][x] = data.depth;

        uniform.window.setPixelColour(x, y, data.colour.asARGB());
    }
}

void FragmentShaders::depth(CanvasTriangle triangle, const int x, const int y,
                            const FragmentData::FilledDataUniform& uniform,
                            const FragmentData::FilledData& data)
{
    if (data.depth > 0 && data.depth < 0.7 &&
        x >= 0 && x < uniform.window.width &&
        y >= 0 && y < uniform.window.height &&
        data.depth > uniform.depthBuffer[y][x])
    {
        auto multiplier = glm::clamp(data.depth, 0.0f, 1.0f) * 255 * 3;

        uniform.depthBuffer[y][x] = data.depth;

        uniform.window.setPixelColour(x, y, Colour(multiplier, multiplier, multiplier).asARGB());
    }
}

void FragmentShaders::material(CanvasTriangle triangle, const int x, const int y,
                               const FragmentData::TextureDataUniform& uniform,
                               const FragmentData::TextureData& data)
{
    if (data.depth > 0 && data.depth < 0.7 &&
        x >= 0 && x < uniform.window.width &&
        y >= 0 && y < uniform.window.height &&
        data.depth > uniform.depthBuffer[y][x])
    {
        uniform.depthBuffer[y][x] = data.depth;
        const auto texture = uniform.textureMap.pixels.at(
            glm::floor(data.texturePoint.y / data.depth) * static_cast<float>(uniform.textureMap.width) + glm::floor(
                data.texturePoint.x / data.depth));

        uniform.window.setPixelColour(x, y, (0xFF << 24) + (texture & 0xFFFFFF));
    }
}
