#include "Interpolation.h"

#include <glm/glm.hpp>

std::vector<float> Interpolation::interpolateSingleFloats(const float from, const float to, const int count)
{
    const float difference = (to - from) / static_cast<float>(count - 1);
    std::vector<float> result;

    float x = from;
    for (float i = 0; i < count; i++)
    {
        x += difference;
        result.push_back(x);
    }

    return result;
}

float Interpolation::proportion(const float from, const float to, const float current, const float fallback = 1)
{
    if (to - from == 0) return fallback;
    return (current - from) / (to - from);
}

float Interpolation::interpolateSingleFloat(const float from, const float to, const float proportion)
{
    return from + (to - from) * proportion;
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