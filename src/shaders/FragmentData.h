//
// Created by Thomas Parr on 28/10/2024.
//

#ifndef POINTDATA_H
#define POINTDATA_H
#include <Colour.h>
#include <DrawingWindow.h>
#include <TexturePoint.h>
#include <glm/glm.hpp>

#include "../helper/Material.h"

namespace FragmentData
{
    struct FilledData {
        glm::vec3 proportion;
        float depth;

        FilledData(glm::vec3 proportion, float depth);

        FilledData operator+(const FilledData &other) const;
        FilledData operator-(const FilledData &other) const;

        template<typename N>
        FilledData operator*(const N num)
        {
            return FilledData(proportion * num, depth * num);
        }
    };

    struct TextureData {
        TexturePoint texturePoint;
        glm::vec3 proportion;
        float depth;

        TextureData(TexturePoint texturePoint, glm::vec3 proportion, float depth);

        TextureData operator+(const TextureData &other) const;
        TextureData operator-(const TextureData &other) const;

        template<typename N>
        TextureData operator*(const N num)
        {
            return TextureData(texturePoint * num, proportion * num, depth * num);
        }
    };

    struct DataUniform
    {
        DrawingWindow& window;
        float** depthBuffer;
        Material material;

        explicit DataUniform(DrawingWindow &window, float** depthBuffer, const Material& material);
    };
}

#endif //POINTDATA_H
