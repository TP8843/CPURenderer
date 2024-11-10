#include "Camera.h"

#include "../helper/constants.h"

Camera::Camera(const glm::vec3 position,
               const glm::mat3& rotation,
               const float focalLength,
               const float imagePlaneScaling)
    : position(position),
      rotation(rotation),
      focalLength(focalLength),
      imagePlaneScaling(imagePlaneScaling),
      initialPosition(glm::vec3(position)),
      initialRotation(glm::mat3(rotation)),
      prevTime(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()))
{
}

glm::mat3 Camera::getNormalRotationMatrix() const
{
    return glm::transpose(glm::inverse(rotation));
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
    rotation = newRotation * rotation;
}

void Camera::rotateX(const float angle)
{
    // Rotate about camera x-axis
    rotation *= glm::mat3(
        glm::vec3(1, 0, 0),
        glm::vec3(0, glm::cos(angle), glm::sin(angle)),
        glm::vec3(0, -glm::sin(angle), glm::cos(angle)));
}

void Camera::rotateY(const float angle)
{
    // Rotate about global y-axis
    rotation = glm::mat3(
        glm::vec3(glm::cos(angle), 0, -glm::sin(angle)),
        glm::vec3(0, 1, 0),
        glm::vec3(glm::sin(angle), 0, glm::cos(angle))) * rotation;
}

bool Camera::toggleOrbit()
{
    orbit = !orbit;
    if (orbit) prevTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch());

    return orbit;
}

void Camera::iterateOrbit()
{
    if (orbit)
    {
        const std::chrono::milliseconds time = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch());

        const float timeElapsed = static_cast<float>((time - prevTime).count()) / 1000.0f;

        const auto rotationMatrix = glm::mat3(
       glm::vec3(glm::cos(constants::speed::ORBIT_SPEED * timeElapsed), 0, -glm::sin(constants::speed::ORBIT_SPEED * timeElapsed)),
       glm::vec3(0, 1, 0),
       glm::vec3(glm::sin(constants::speed::ORBIT_SPEED * timeElapsed), 0, glm::cos(constants::speed::ORBIT_SPEED * timeElapsed)));

        position = rotationMatrix * position;
        lookAt(glm::vec3(0,0,0));

        prevTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch());
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

