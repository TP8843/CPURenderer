#ifndef MODELPOINT_H
#define MODELPOINT_H
#include <TexturePoint.h>
#include <glm/glm.hpp>


struct ModelPoint {
    glm::vec3 position;
    glm::vec3 normal;
    TexturePoint texturePoint;

    ModelPoint() = default;
    explicit ModelPoint(glm::vec3 position);
    ModelPoint(glm::vec3 position, TexturePoint texturePoint);
    ModelPoint(glm::vec3 position, TexturePoint texturePoint, glm::vec3 normal);

    ModelPoint operator+(const ModelPoint &other) const;
    ModelPoint operator-(const ModelPoint &other) const;

    template<typename N>
    ModelPoint operator*(const N num) const
    {
        return ModelPoint(position * num, texturePoint * num, normal * num);
    }

    template<typename N>
ModelPoint operator/(const N num) const
    {
        return ModelPoint(position / num, texturePoint / num, normal / num);
    }
};



#endif //MODELPOINT_H
