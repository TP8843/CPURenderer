#include "Transformation.h"

#include <iostream>

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

void Transformation::lookAt(const glm::vec3 location)
{
    const auto zRotation = glm::normalize(position - location);
    const auto xRotation = glm::normalize(glm::cross(glm::vec3(0, 1, 0), zRotation));
    const auto yRotation = glm::normalize(glm::cross(zRotation, xRotation));

    rotation = glm::mat3(xRotation, yRotation, zRotation);
}

void Transformation::translateAbsolute(const glm::vec3 &translation)
{
    position += translation;
}

void Transformation::translateRelative(const glm::vec3 &translation)
{
    position += rotation * translation;
}

void Transformation::rotate(const glm::mat3& newRotation)
{
    rotation = newRotation * rotation;
}

void Transformation::rotateX(const float angle)
{
    // Rotate about camera x-axis
    rotation *= glm::mat3(
        glm::vec3(1, 0, 0),
        glm::vec3(0, glm::cos(angle), glm::sin(angle)),
        glm::vec3(0, -glm::sin(angle), glm::cos(angle)));
}

void Transformation::rotateY(const float angle)
{
    // Rotate about global y-axis
    rotation = glm::mat3(
        glm::vec3(glm::cos(angle), 0, -glm::sin(angle)),
        glm::vec3(0, 1, 0),
        glm::vec3(glm::sin(angle), 0, glm::cos(angle))) * rotation;
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