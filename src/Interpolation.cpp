//
// Created by Thomas Parr on 24/09/2024.
//

#include "Interpolation.h"

#include <glm/glm.hpp>

std::vector<float> Interpolation::interpolateSingleFloats(float from, float to, int count)
{
    const float difference = (to - from) / (count - 1);
    std::vector<float> result;

    for (int i = 0; i < count; i++)
    {
        result.push_back(from + (difference * i));
    }

    return result;
}

std::vector<glm::vec3> Interpolation::interpolateThreeElementValues(const glm::vec3 from, const glm::vec3 to, const int count)
{
    const auto xs = interpolateSingleFloats(from.x, to.x, count);
    const auto ys = interpolateSingleFloats(from.y, to.y, count);
    const auto zs = interpolateSingleFloats(from.z, to.z, count);

    std::vector<glm::vec3> result;
    for (int i = 0; i < count; i++)
    {
        auto vector = glm::vec3(xs[i], ys[i], zs[i]);
        result.push_back(vector);
    }

    return result;
}