#include "Camera.h"

#include "constants.h"

Camera::Camera(const glm::vec3 position,
               const glm::mat3& rotation,
               const float focalLength,
               const float imagePlaneScaling)
    : position(position),
      rotation(rotation),
      focalLength(focalLength),
      imagePlaneScaling(imagePlaneScaling),
      initialPosition(glm::vec3(position)),
      initialRotation(glm::mat3(rotation))
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

bool Camera::toggleOrbit()
{
    orbit = !orbit;

    return orbit;
}

void Camera::iterateOrbit()
{
    if (orbit)
    {
        const auto rotationMatrix = glm::mat3(
       glm::vec3(glm::cos(constants::speed::ORBIT_SPEED), 0, -glm::sin(constants::speed::ORBIT_SPEED)),
       glm::vec3(0, 1, 0),
       glm::vec3(glm::sin(constants::speed::ORBIT_SPEED), 0, glm::cos(constants::speed::ORBIT_SPEED)));

        position = rotationMatrix * position;
        lookAt(glm::vec3(0,0,0));
    }
}

bool Camera::getOrbit() const
{
    return orbit;
}

void Camera::reset()
{
    position = glm::vec3(initialPosition);
    rotation = glm::mat3(initialRotation);
}

void Camera::lookAt(const glm::vec3 location = glm::vec3(0,0,0))
{
    const auto zRotation = glm::normalize(position - location);
    const auto xRotation = glm::normalize(glm::cross(glm::vec3(0, 1, 0), zRotation));
    const auto yRotation = glm::normalize(glm::cross(zRotation, xRotation));

    rotation = glm::mat3(xRotation, yRotation, zRotation);
}

