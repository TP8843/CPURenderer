#include "Transformation.h"

Transformation::Transformation(const glm::vec3 &position, const glm::mat3 &rotation, const float &scale) :
    position(position), rotation(rotation), scale(scale),
    initialPosition(position), initialRotation(rotation), initialScale(scale)
{
}

glm::vec3 Transformation::transformPoint(const glm::vec3 point) const
{
    return (point * scale + position) * rotation;
}

glm::mat3 Transformation::getNormalRotationMatrix() const
{
    return glm::transpose(glm::inverse(rotation));
}

void Transformation::reset(){
    scale = initialScale;
    position = initialPosition;
    rotation = initialRotation;
}

void Transformation::translateAbsolute(const glm::vec3 &translation)
{
    position += translation;
}

void Transformation::translateRelative(const glm::vec3 &translation)
{
    position += rotation * translation;
}

Transformation Transformation::operator+(const Transformation &other) const
{
    return {
        position + other.position,
        rotation + other.rotation,
        scale + other.scale
    };
}

Transformation Transformation::operator-(const Transformation &other) const
{
    return {
        position - other.position,
        rotation - other.rotation,
        scale - other.scale
    };
}