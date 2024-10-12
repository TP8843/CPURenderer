#ifndef INTERPOLATION_H
#define INTERPOLATION_H
#include <vector>
#include <glm/glm.hpp>


class Interpolation {
public: static std::vector<float> interpolateSingleFloats(float, float, int);

public: static float proportion(float from, float to, float current, float fallback);
public: static float interpolateSingleFloat(float from, float to, float proportion);

public: static std::vector<glm::vec3> interpolateThreeElementValues(glm::vec3, glm:: vec3, int);
};



#endif //INTERPOLATION_H
