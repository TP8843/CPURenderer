#ifndef INTERPOLATION_H
#define INTERPOLATION_H
#include <vector>
#include <../../libs/glm-0.9.7.2/glm/glm.hpp>


class Interpolation {
public:
    static std::vector<float> interpolateSingleFloats(float, float, int);

    static float proportion(float from, float to, float current, float fallback);

    template <typename T>
    static T interpolate(T from, T to, float proportion)
    {
        return from + (to - from) * proportion;
    }

    static std::vector<glm::vec3> interpolateThreeElementValues(glm::vec3, glm:: vec3, int);
};



#endif //INTERPOLATION_H
