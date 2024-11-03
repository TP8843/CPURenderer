#ifndef LIGHT_H
#define LIGHT_H
#include <glm/glm.hpp>

#include "Camera.h"


struct Light {
    glm::vec3 position;
    float intensity;

    explicit Light(glm::vec3 position, float intensity);

    // Get multiplier (point must be in camera space)
    float getMultiplier(const Camera& camera, glm::vec3 point, glm::vec3 normal) const;

private:
    static float calculateMultiplier(glm::vec3 lightPosition,
        float intensity,
        glm::vec3 point,
        glm::vec3 normal);

};



#endif //LIGHT_H
