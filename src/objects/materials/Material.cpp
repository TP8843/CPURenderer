#include "Material.h"

Colour Material::getColour() const {
    return colour;
}

IlluminationModel Material::getIlluminationModel() const {
    return illuminationModel;
}

bool Material::hasTexture() const {
    return hasTextureBool;
}

Colour Material::getPixelTextureColour(const size_t x, const size_t y) const {
    const uint32_t rawColour = texture.pixels[y * texture.width + x];

    return {
        static_cast<int>(rawColour & 0xFF),
        static_cast<int>(rawColour >> 8 & 0xFF),
        static_cast<int>(rawColour >> 16 & 0xFF)
    };
}

size_t Material::getTextureWidth() const {
    return texture.width;
}

size_t Material::getTextureHeight() const {
    return texture.height;
}

float Material::getColourAtPointInCameraSpace(
                                 const Camera &camera,
                                 const Light &light,
                                 const glm::vec3 &point,
                                 const glm::vec3 &normal) const
{
    const auto lightDisplacement = point - light.getPositionInCameraSpace(camera);
    const auto normalisedLightDisplacement = glm::normalize(lightDisplacement);

    const auto reflectedDirection = normalisedLightDisplacement
        - 2.0f * normal * glm::dot(normalisedLightDisplacement, normal);

    const float specularIntensity = glm::pow(
        glm::max(glm::dot(glm::normalize(-point), glm::normalize(reflectedDirection)), 0.0f), specularStrength);

    constexpr float ambientIntensity = 0.2f;

    const float diffuseIntensity = glm::clamp((glm::dot(normal, normalisedLightDisplacement)) /
        (1.0f + (1.0f * glm::pow(glm::length(lightDisplacement), 1.0f))), 0.0f, 1.0f);

    return glm::clamp(ambientIntensity + light.intensity * (diffuseIntensity + specularIntensity), 0.0f,  1.0f);
}

Material::Material() = default;

Material::Material(Colour colour, const IlluminationModel illuminationModel,
                   const float specularStrength): colour(std::move(colour)), illuminationModel(illuminationModel),
                                                  specularStrength(specularStrength) {
}

Material::Material(Colour colour, const IlluminationModel illuminationModel, const float specularStrength,
                   TextureMap texture) : colour(std::move(colour)), illuminationModel(illuminationModel),
                                         specularStrength(specularStrength), texture(std::move(texture)),
                                         hasTextureBool(true) {
}
