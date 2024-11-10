#include "Light.h"

Light::Light(const glm::vec3 position, const float intensity) :
    position(position), intensity(intensity)
{}

glm::vec3 Light::getPositionInCameraSpace(const Camera& camera) const
{
    return (position - camera.position) * camera.rotation;
}