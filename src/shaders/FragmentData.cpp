#include "FragmentData.h"

#include "../objects/Transformation.h"

FragmentData::FilledData::FilledData(const glm::vec3 proportion,
                                     const float depth,
                                     const glm::vec3 position3D,
                                     const glm::vec3 normal) :
    proportion(proportion), depth(depth), position3D(position3D), normal(normal)
{
}

FragmentData::FilledData FragmentData::FilledData::operator+(const FilledData& other) const
{
    return {
        proportion + other.proportion,
        depth + other.depth,
        position3D + other.position3D,
        normal + other.normal
    };
}

FragmentData::FilledData FragmentData::FilledData::operator-(const FilledData& other) const
{
    return {
        proportion - other.proportion,
        depth - other.depth,
        position3D - other.position3D,
        normal - other.normal
    };
}

FragmentData::TextureData::TextureData(const TexturePoint texturePoint,
                                       const glm::vec3 proportion,
                                       const float depth,
                                       const glm::vec3 position3D,
                                       const glm::vec3 normal) :
    texturePoint(texturePoint), proportion(proportion), depth(depth), position3D(position3D), normal(normal)
{
}

FragmentData::TextureData FragmentData::TextureData::operator+(const TextureData& other) const
{
    return {
        texturePoint + other.texturePoint,
        proportion + other.proportion,
        depth + other.depth,
        position3D + other.position3D,
        normal + other.normal
    };
}

FragmentData::TextureData FragmentData::TextureData::operator-(const TextureData& other) const
{
    return {
        texturePoint - other.texturePoint,
        proportion - other.proportion,
        depth - other.depth,
        position3D - other.position3D,
        normal - other.normal,
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
                                       Transformation& camera,
                                       Transformation& light,
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
