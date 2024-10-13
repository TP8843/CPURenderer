#include "Camera.h"

Camera::Camera(const glm::vec3 position,
               const glm::mat3& rotation,
               const float focalLength,
               const float imagePlaneScaling)
    : position(position),
      rotation(rotation),
      focalLength(focalLength),
      imagePlaneScaling(imagePlaneScaling)
{
}

// Translate taking into account camera rotation
void Camera::translateRelative(const glm::vec3 translation)
{
    position += rotation * translation;
}

// Translate ignoring camera rotation
void Camera::translateAbsolute(const glm::vec3 translation)
{
    position += translation;
}

void Camera::rotate(const glm::mat3& newRotation)
{
    rotation *= newRotation;
}

void Camera::rotateX(const float angle)
{
    rotation *= glm::mat3(
        glm::vec3(1, 0, 0),
        glm::vec3(0, glm::cos(angle), glm::sin(angle)),
        glm::vec3(0, -glm::sin(angle), glm::cos(angle)));
}

void Camera::rotateY(const float angle)
{
    rotation *= glm::mat3(
        glm::vec3(glm::cos(angle), 0, -glm::sin(angle)),
        glm::vec3(0, 1, 0),
        glm::vec3(glm::sin(angle), 0, glm::cos(angle)));
}
