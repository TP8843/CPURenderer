#ifndef LIGHT_H
#define LIGHT_H
#include <glm/glm.hpp>

#include "Camera.h"


struct Light {
    glm::vec3 position;
    float intensity;

    explicit Light(glm::vec3 position, float intensity);

    glm::vec3 getPositionInCameraSpace(const Camera& camera) const;
};



#endif //LIGHT_H
