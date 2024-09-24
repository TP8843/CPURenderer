//
// Created by Thomas Parr on 24/09/2024.
//

#ifndef INTERPOLATION_H
#define INTERPOLATION_H
#include <vector>
#include <glm/glm.hpp>


class Interpolation {
public: static std::vector<float> interpolateSingleFloats(float, float, int);
public: static std::vector<glm::vec3> interpolateThreeElementValues(glm::vec3, glm:: vec3, int);
};



#endif //INTERPOLATION_H
