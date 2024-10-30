//
// Created by Thomas Parr on 28/10/2024.
//

#include "FragmentData.h"

#include <utility>

FragmentData::FilledData::FilledData(const glm::vec3 proportion, const float depth) :
        proportion(proportion), depth(depth)
{}

FragmentData::FilledData FragmentData::FilledData::operator+(const FilledData& other) const
{
        return {
                proportion + other.proportion,
                depth + other.depth
        };
}

FragmentData::FilledData FragmentData::FilledData::operator-(const FilledData& other) const
{
        return {
                proportion - other.proportion,
                depth - other.depth
        };
}

FragmentData::TextureData::TextureData(const TexturePoint texturePoint, const glm::vec3 proportion, const float depth) :
        texturePoint(texturePoint), proportion(proportion), depth(depth)
{}

FragmentData::TextureData FragmentData::TextureData::operator+(const TextureData &other) const
{
        return {
                texturePoint + other.texturePoint,
                proportion + other.proportion,
                depth + other.depth };
}

FragmentData::TextureData FragmentData::TextureData::operator-(const TextureData &other) const
{
        return { texturePoint - other.texturePoint,
                proportion - other.proportion,
                depth - other.depth };
}

FragmentData::DataUniform::DataUniform(DrawingWindow& window, float** depthBuffer, const Material& material) :
        window(window), depthBuffer(depthBuffer), material(material)
{}
