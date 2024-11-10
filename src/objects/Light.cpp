#include "Light.h"

Light::Light(const glm::vec3 position, const float intensity) :
    position(position), intensity(intensity)
{}

float Light::getMultiplier(const Camera& camera, const glm::vec3 point, const glm::vec3 normal) const
{
    const auto transformedLight = (position - camera.position) * camera.rotation;

    return calculateMultiplier(transformedLight, intensity, point, normal);
}

float Light::calculateMultiplier(const glm::vec3 lightPosition, const float intensity, const glm::vec3 point, const glm::vec3 normal)
{
    const auto lightDisplacement = point - lightPosition;
    const auto normalizedLightDisplacement = glm::normalize(lightDisplacement);

    const auto reflectedDirection = normalizedLightDisplacement
        - 2.0f * normal * glm::dot(normalizedLightDisplacement, normal);

    const float specularIntensity = static_cast<float>(glm::pow(
        glm::max(glm::dot(glm::normalize(-point), glm::normalize(reflectedDirection)), 0.0f), 256));

    constexpr float ambientIntensity = 0.2f;

    const float diffuseIntensity = glm::clamp((glm::dot(normal, normalizedLightDisplacement)) /
        (1.0f + (1.0f * glm::pow(glm::length(lightDisplacement), 1.0f))), 0.0f, 1.0f);

    return glm::clamp(ambientIntensity + intensity * (diffuseIntensity + specularIntensity), 0.0f,  1.0f);
}
