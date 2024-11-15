#include "Material.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../Transformation.h"
#include "../../libs/stb_image.h"

Colour Material::getColour() const
{
    return colour;
}

IlluminationModel Material::getIlluminationModel() const
{
    return illuminationModel;
}

bool Material::hasTexture() const
{
    return hasTextureBool;
}

Colour Material::getPixelTextureColour(const int x, const int y) const
{
    if (x < 0 || x >= textureWidth || y < 0 || y >= textureHeight)
    {
        std::cout << "Texture out of bounds at (" << x << "," << y << ")" << std::endl;
        return {0, 0, 0};
    }

    const int startingPosition = (textureWidth * y + x) * charsPerPixel;

    return {
        static_cast<int>(texture[startingPosition + 0]),
        static_cast<int>(texture[startingPosition + 1]),
        static_cast<int>(texture[startingPosition + 2]),
    };
}

size_t Material::getTextureWidth() const
{
    return textureWidth;
}

size_t Material::getTextureHeight() const
{
    return textureHeight;
}

float Material::getColourAtPointInCameraSpace(
    const Transformation& camera,
    const Transformation& light,
    const glm::vec3& point,
    const glm::vec3& normal) const
{
    const auto lightDisplacement = point - (light.position - camera.position) * camera.rotation;
    const auto normalisedLightDisplacement = glm::normalize(lightDisplacement);

    const auto reflectedDirection = normalisedLightDisplacement
        - 2.0f * normal * glm::dot(normalisedLightDisplacement, normal);

    const float specularIntensity = glm::pow(
        glm::max(glm::dot(glm::normalize(-point), glm::normalize(reflectedDirection)), 0.0f), specularStrength);

    constexpr float ambientIntensity = 0.2f;


    float diffuseIntensity = 0.0f;

    if (glm::dot(point, normal) > 0.0f)
    {
        diffuseIntensity = glm::clamp((glm::dot(normal, normalisedLightDisplacement)) /
                                      (1.0f + 1.0f * glm::pow(glm::length(lightDisplacement), 2.0f)), 0.0f, 1.0f);
    }

    const float total = ambientIntensity + light.scale * (diffuseIntensity + specularIntensity);

    return glm::clamp(total / (1 + total), 0.0f, 1.0f);
}

Material::Material() = default;

Material::Material(Colour colour,
                   const IlluminationModel illuminationModel,
                   const float specularStrength) :
    colour(std::move(colour)),
    illuminationModel(illuminationModel),
    specularStrength(specularStrength)
{
}

Material::Material(const Colour& colour,
                   const IlluminationModel illuminationModel,
                   const float specularStrength,
                   const std::string& texturePath) :
    colour(colour),
    illuminationModel(illuminationModel),
    specularStrength(specularStrength),
    hasTextureBool(true)
{
    std::cout << "Attempting to load texture from " << texturePath << std::endl;

    texture = stbi_load(texturePath.c_str(),
                         &textureWidth,
                         &textureHeight,
                         nullptr,
                         STBI_rgb_alpha);
}
