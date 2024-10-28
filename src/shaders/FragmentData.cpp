//
// Created by Thomas Parr on 28/10/2024.
//

#include "FragmentData.h"

#include <utility>

FragmentData::FilledData::FilledData(Colour colour, const glm::vec3 proportion, const float depth) :
        colour(std::move(colour)), proportion(proportion), depth(depth)
{}

FragmentData::FilledData FragmentData::FilledData::operator+(const FilledData& other) const
{
        return {
                colour + other.colour,
                proportion + other.proportion,
                depth + other.depth
        };
}

FragmentData::FilledData FragmentData::FilledData::operator-(const FilledData& other) const
{
        return {
                colour - other.colour,
                proportion - other.proportion,
                depth - other.depth
        };
}

FragmentData::TextureData::TextureData(Colour colour, const TexturePoint texturePoint, const glm::vec3 proportion, const float depth) :
        colour(std::move(colour)), texturePoint(texturePoint), proportion(proportion), depth(depth)
{}

FragmentData::TextureData FragmentData::TextureData::operator+(const TextureData &other) const
{
        return { colour + other.colour,
                texturePoint + other.texturePoint,
                proportion + other.proportion,
                depth + other.depth };
}

FragmentData::TextureData FragmentData::TextureData::operator-(const TextureData &other) const
{
        return { colour - other.colour,
                texturePoint - other.texturePoint,
                proportion - other.proportion,
                depth - other.depth };
}

FragmentData::TextureDataUniform::TextureDataUniform(const TextureMap textureMap) :
        textureMap(textureMap)
{}
