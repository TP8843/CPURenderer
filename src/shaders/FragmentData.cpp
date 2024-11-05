#include "FragmentData.h"

FragmentData::Data::Data(const glm::vec3 proportion,
                                     const float depth,
                                     const ModelPoint& point) :
    proportion(proportion), depth(depth), point(point)
{
}

FragmentData::Data FragmentData::Data::operator+(const Data& other) const
{
    return {
        proportion + other.proportion,
        depth + other.depth,
        point + other.point
    };
}

FragmentData::Data FragmentData::Data::operator-(const Data& other) const
{
    return {
        proportion - other.proportion,
        depth - other.depth,
        point - other.point
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
