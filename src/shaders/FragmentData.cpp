#include "FragmentData.h"

FragmentData::FilledData::FilledData(const glm::vec3 proportion,
                                     const float depth,
                                     const glm::vec3 position3D) :
    proportion(proportion), depth(depth), position3D(position3D)
{
}

FragmentData::FilledData FragmentData::FilledData::operator+(const FilledData& other) const
{
    return {
        proportion + other.proportion,
        depth + other.depth,
        position3D + other.position3D
    };
}

FragmentData::FilledData FragmentData::FilledData::operator-(const FilledData& other) const
{
    return {
        proportion - other.proportion,
        depth - other.depth,
        position3D - other.position3D
    };
}

FragmentData::TextureData::TextureData(const TexturePoint texturePoint,
                                       const glm::vec3 proportion,
                                       const float depth,
                                       const glm::vec3 position3D) :
    texturePoint(texturePoint), proportion(proportion), depth(depth), position3D(position3D)
{
}

FragmentData::TextureData FragmentData::TextureData::operator+(const TextureData& other) const
{
    return {
        texturePoint + other.texturePoint,
        proportion + other.proportion,
        depth + other.depth,
        position3D + other.position3D
    };
}

FragmentData::TextureData FragmentData::TextureData::operator-(const TextureData& other) const
{
    return {
        texturePoint - other.texturePoint,
        proportion - other.proportion,
        depth - other.depth,
        position3D - other.position3D
    };
}

FragmentData::PrePassData::PrePassData(const float depth) :
    depth(depth)
{
}

FragmentData::PrePassData FragmentData::PrePassData::operator+(const PrePassData& other) const
{
    return PrePassData(depth + other.depth);
}

FragmentData::PrePassData FragmentData::PrePassData::operator-(const PrePassData& other) const
{
    return PrePassData(depth - other.depth);
}

FragmentData::DataUniform::DataUniform(DrawingWindow& window,
                                       float** depthBuffer,
                                       Material& material,
                                       Camera& camera,
                                       Light& light,
                                       const glm::vec3& normal) :
    window(window),
    depthBuffer(depthBuffer),
    material(material),
    camera(camera),
    light(light),
    normal(normal)
{
}

FragmentData::PrePassUniform::PrePassUniform(float** depthBuffer, const size_t& width, const size_t& height) :
    depthBuffer(depthBuffer), width(width), height(height)
{
}
