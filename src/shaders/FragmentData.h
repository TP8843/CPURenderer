//
// Created by Thomas Parr on 28/10/2024.
//

#ifndef POINTDATA_H
#define POINTDATA_H
#include <Colour.h>
#include <TextureMap.h>
#include <TexturePoint.h>
#include <glm/glm.hpp>

namespace FragmentData
{
    struct FilledData {
        Colour colour;
        glm::vec3 proportion;
        float depth;

        FilledData(Colour  colour, glm::vec3 proportion, float depth);

        FilledData operator+(const FilledData &other) const;
        FilledData operator-(const FilledData &other) const;

        template<typename N>
        FilledData operator*(const N num)
        {
            return FilledData(colour * num, proportion * num, depth * num);
        }
    };

    struct FilledDataUniform
    {
        FilledDataUniform() = default;
    };

    struct TextureData {
        Colour colour;
        TexturePoint texturePoint;
        glm::vec3 proportion;
        float depth;

        TextureData(Colour  colour, TexturePoint texturePoint, glm::vec3 proportion, float depth);

        TextureData operator+(const TextureData &other) const;
        TextureData operator-(const TextureData &other) const;

        template<typename N>
        TextureData operator*(const N num)
        {
            return TextureData(colour * num, texturePoint * num, proportion * num, depth * num);
        }
    };

    struct TextureDataUniform
    {
        TextureMap textureMap;

        explicit TextureDataUniform(TextureMap textureMap);
    };
}

#endif //POINTDATA_H
