#include "FragmentShaders.h"

void FragmentShaders::prePass(const CanvasTriangle& _,
                              const int x, const int y,
                              const FragmentData::PrePassUniform& uniform,
                              const FragmentData::PrePassData& data)
{
    if (data.depth > 0 && data.depth < 0.7 &&
    x >= 0 && x < static_cast<int>(uniform.width) &&
    y >= 0 && y < static_cast<int>(uniform.height) &&
    data.depth > uniform.depthBuffer[y][x])
    {
        uniform.depthBuffer[y][x] = data.depth;
    }
}


void FragmentShaders::filled(const CanvasTriangle& _,
                             const int x, const int y,
                             const FragmentData::DataUniform& uniform,
                             const FragmentData::FilledData& data)
{
    if (data.depth > 0 && data.depth < 0.7 &&
        x >= 0 && x < static_cast<int>(uniform.window.width) &&
        y >= 0 && y < static_cast<int>(uniform.window.height) &&
        data.depth == uniform.depthBuffer[y][x])
    {

        const Colour colour = uniform.material.getColourAtPointInCameraSpace(
            uniform.camera,
            uniform.light,
            data.position3D / data.depth,
            uniform.normal,
            uniform.material.getIlluminationModel());

        uniform.window.setPixelColour(x, y, colour.asARGB());
    }
}

void FragmentShaders::filledPhong(const CanvasTriangle& _,
                             const int x, const int y,
                             const FragmentData::DataUniform& uniform,
                             const FragmentData::FilledData& data)
{
    if (data.depth > 0 && data.depth < 0.7 &&
        x >= 0 && x < static_cast<int>(uniform.window.width) &&
        y >= 0 && y < static_cast<int>(uniform.window.height) &&
        data.depth == uniform.depthBuffer[y][x])
    {

        const Colour colour = uniform.material.getColourAtPointInCameraSpace(
            uniform.camera,
            uniform.light,
            data.position3D / data.depth,
            data.normal / data.depth,
            uniform.material.getIlluminationModel());

        uniform.window.setPixelColour(x, y, colour.asARGB());
    }
}

void FragmentShaders::rainbow(const CanvasTriangle& _,
                              const int x, const int y,
                              const FragmentData::DataUniform& uniform,
                              const FragmentData::FilledData& data)
{
    if (data.depth > 0 && data.depth < 0.7 &&
        x >= 0 && x < static_cast<int>(uniform.window.width) &&
        y >= 0 && y < static_cast<int>(uniform.window.height) &&
        data.depth == uniform.depthBuffer[y][x])
    {
        uniform.window.setPixelColour(
            x, y, Colour(data.proportion[0] * 255, data.proportion[1] * 255, data.proportion[2] * 255).asARGB());
    }
}

void FragmentShaders::outline(const CanvasTriangle& _,
                              const int x, const int y,
                              const FragmentData::DataUniform& uniform,
                              const FragmentData::FilledData& data)
{
    if (data.depth > 0 && data.depth < 0.7 &&
        x >= 0 && x < static_cast<int>(uniform.window.width) &&
        y >= 0 && y < static_cast<int>(uniform.window.height) &&
        data.depth == uniform.depthBuffer[y][x]
        && glm::min(data.proportion[0], glm::min(data.proportion[1], data.proportion[2])) < 0.5 * data.depth
    )
    {
        uniform.window.setPixelColour(x, y, uniform.material.getColour().asARGB());
    }
}

void FragmentShaders::depth(const CanvasTriangle& _,
                            const int x, const int y,
                            const FragmentData::DataUniform& uniform,
                            const FragmentData::FilledData& data)
{
    if (data.depth > 0 && data.depth < 0.7 &&
        x >= 0 && x < static_cast<int>(uniform.window.width) &&
        y >= 0 && y < static_cast<int>(uniform.window.height) &&
        data.depth == uniform.depthBuffer[y][x])
    {
        const float multiplier = glm::clamp(data.depth, 0.0f, 1.0f) * 1000.0f;

        uniform.window.setPixelColour(x, y, Colour(multiplier, multiplier, multiplier).asARGB());
    }
}

void FragmentShaders::material(const CanvasTriangle& _,
                               const int x, const int y,
                               const FragmentData::DataUniform& uniform,
                               const FragmentData::TextureData& data)
{
    if (data.depth > 0 && data.depth < 0.7 &&
        x >= 0 && x < static_cast<int>(uniform.window.width) &&
        y >= 0 && y < static_cast<int>(uniform.window.height) &&
        data.depth == uniform.depthBuffer[y][x])
    {

        const Colour colour = uniform.material.getColourAtPointInCameraSpace(
                uniform.camera,
                uniform.light,
                data.position3D / data.depth,
                uniform.normal,
                data.texturePoint / data.depth,
                uniform.material.getIlluminationModel());

        uniform.window.setPixelColour(x, y, colour.asARGB());
    }
}

void FragmentShaders::materialPhong(const CanvasTriangle& _,
                                    const int x, const int y,
                                    const FragmentData::DataUniform& uniform,
                                    const FragmentData::TextureData& data)
{
    if (data.depth > 0 && data.depth < 0.7 &&
    x >= 0 && x < static_cast<int>(uniform.window.width) &&
    y >= 0 && y < static_cast<int>(uniform.window.height) &&
    data.depth == uniform.depthBuffer[y][x])
    {

        const Colour colour = uniform.material.getColourAtPointInCameraSpace(
            uniform.camera,
            uniform.light,
            data.position3D / data.depth,
            data.normal / data.depth,
            data.texturePoint / data.depth,
            uniform.material.getIlluminationModel());

        uniform.window.setPixelColour(x, y, colour.asARGB());
    }
}
