//
// Created by Thomas Parr on 28/10/2024.
//

#include "FragmentShaders.h"

void FragmentShaders::prePass(CanvasTriangle triangle,
                              const int x, const int y,
                              const FragmentData::PrePassUniform& uniform,
                              const FragmentData::PrePassData& data)
{
    if (data.depth > 0 && data.depth < 0.7 &&
    x >= 0 && x < uniform.width &&
    y >= 0 && y < uniform.height &&
    data.depth > uniform.depthBuffer[y][x])
    {
        uniform.depthBuffer[y][x] = data.depth;
    }
}


void FragmentShaders::filled(CanvasTriangle triangle,
                             const int x, const int y,
                             const FragmentData::DataUniform& uniform,
                             const FragmentData::FilledData& data)
{
    if (data.depth > 0 && data.depth < 0.7 &&
        x >= 0 && x < uniform.window.width &&
        y >= 0 && y < uniform.window.height &&
        data.depth == uniform.depthBuffer[y][x])
    {
        const float multiplier =
            uniform.light.getMultiplier(uniform.camera, data.position3D / data.depth, uniform.normal);

        uniform.window.setPixelColour(x, y, (uniform.material.getColour() * multiplier).asARGB());
    }
}

void FragmentShaders::rainbow(CanvasTriangle triangle, const int x, const int y,
                              const FragmentData::DataUniform& uniform,
                              const FragmentData::FilledData& data)
{
    if (data.depth > 0 && data.depth < 0.7 &&
        x >= 0 && x < uniform.window.width &&
        y >= 0 && y < uniform.window.height &&
        data.depth == uniform.depthBuffer[y][x])
    {
        uniform.window.setPixelColour(
            x, y, Colour(data.proportion[0] * 255, data.proportion[1] * 255, data.proportion[2] * 255).asARGB());
    }
}

void FragmentShaders::outline(CanvasTriangle triangle, const int x, const int y,
                              const FragmentData::DataUniform& uniform,
                              const FragmentData::FilledData& data)
{
    if (data.depth > 0 && data.depth < 0.7 &&
        x >= 0 && x < uniform.window.width &&
        y >= 0 && y < uniform.window.height &&
        data.depth == uniform.depthBuffer[y][x]
        && glm::min(data.proportion[0], glm::min(data.proportion[1], data.proportion[2])) < 0.5 * data.depth
    )
    {
        uniform.window.setPixelColour(x, y, uniform.material.getColour().asARGB());
    }
}

void FragmentShaders::depth(CanvasTriangle triangle, const int x, const int y,
                            const FragmentData::DataUniform& uniform,
                            const FragmentData::FilledData& data)
{
    if (data.depth > 0 && data.depth < 0.7 &&
        x >= 0 && x < uniform.window.width &&
        y >= 0 && y < uniform.window.height &&
        data.depth == uniform.depthBuffer[y][x])
    {
        const float multiplier = glm::clamp(data.depth, 0.0f, 1.0f) * 1000.0f;

        uniform.window.setPixelColour(x, y, Colour(multiplier, multiplier, multiplier).asARGB());
    }
}

void FragmentShaders::material(CanvasTriangle triangle, const int x, const int y,
                               const FragmentData::DataUniform& uniform,
                               const FragmentData::TextureData& data)
{
    if (data.depth > 0 && data.depth < 0.7 &&
        x >= 0 && x < uniform.window.width &&
        y >= 0 && y < uniform.window.height &&
        data.depth == uniform.depthBuffer[y][x])
    {
        const float multiplier =
            uniform.light.getMultiplier(uniform.camera,
                                        data.position3D / data.depth,
                                        uniform.normal);

        const auto texture = (uniform.material.getPixelTextureColour(
            glm::floor(data.texturePoint.x / data.depth),
            glm::floor(data.texturePoint.y / data.depth)) * multiplier).asARGB();

        uniform.window.setPixelColour(x, y, texture);
    }
}
