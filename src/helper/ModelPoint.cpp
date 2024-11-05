#include "ModelPoint.h"

ModelPoint::ModelPoint(const glm::vec3 position) :
    position(position)
{
}

ModelPoint::ModelPoint(const glm::vec3 position, const TexturePoint texturePoint) :
    position(position), texturePoint(texturePoint)
{
}

ModelPoint::ModelPoint(const glm::vec3 position, const TexturePoint texturePoint, const glm::vec3 normal) :
    position(position), normal(normal), texturePoint(texturePoint)
{
}

ModelPoint ModelPoint::operator+(const ModelPoint& other) const
{
    return {
        position + other.position,
        texturePoint + other.texturePoint,
        normal + other.normal
    };
}

ModelPoint ModelPoint::operator-(const ModelPoint& other) const
{
    return {
        position - other.position,
        texturePoint - other.texturePoint,
        normal - other.normal
    };
}
