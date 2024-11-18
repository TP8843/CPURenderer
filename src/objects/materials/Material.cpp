#include "Material.h"
#define STB_IMAGE_IMPLEMENTATION
#include <TexturePoint.h>

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
    int tiledX = x % textureWidth;
    int tiledY = y % textureHeight;

    if (tiledX < 0) tiledX += textureWidth;
    if (tiledY < 0) tiledY += textureHeight;

    if (tiledX < 0 || tiledX >= textureWidth || tiledY < 0 || tiledY >= textureHeight)
    {
        std::cout << "Texture out of bounds at (" << tiledX << "," << tiledY << ")" << std::endl;
        return {0, 0, 0};
    }

    const int startingPosition = (textureWidth * tiledY + tiledX) * charsPerPixel;

    return {
        static_cast<float>(texture[startingPosition + 0]),
        static_cast<float>(texture[startingPosition + 1]),
        static_cast<float>(texture[startingPosition + 2]),
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

Colour Material::getColourAtPointInCameraSpace(
    const Transformation& camera,
    const Transformation& light,
    const glm::vec3& point,
    const glm::vec3& normal,
    const TexturePoint& texturePosition,
    const IlluminationModel& illuminationModel,
    const bool inShadow) const
{
    const Colour textureColour = getPixelTextureColour(
        glm::round(texturePosition.x),
        glm::round(texturePosition.y));

    switch (illuminationModel)
    {
    case PHONG: return phongShadedColour(
            camera,
            light,
            point,
            normal,
            specularStrength,
            textureColour,
            textureColour,
            Colour(255, 255, 255),
            inShadow);

    default: return flatShadedColour(
            camera,
            light,
            point,
            normal,
            textureColour,
            textureColour,
            inShadow);
    }
}

Colour Material::getColourAtPointInCameraSpace(
    const Transformation& camera,
    const Transformation& light,
    const glm::vec3& point,
    const glm::vec3& normal,
    const IlluminationModel& illuminationModel,
    const bool inShadow) const
{
    switch (illuminationModel)
    {
    case PHONG: return phongShadedColour(
            camera,
            light,
            point,
            normal,
            specularStrength,
            colour,
            colour,
            Colour(255, 255, 255),
            inShadow);

    default: return flatShadedColour(
            camera,
            light,
            point,
            normal,
            colour,
            colour,
            inShadow);
    }
}

Material::Material() = default;

Material::Material(Colour colour,
                   const IlluminationModel illuminationModel,
                   const float specularStrength) : colour(std::move(colour)),
                                                   illuminationModel(illuminationModel),
                                                   specularStrength(specularStrength)
{
}

Material::Material(const Colour& colour,
                   const std::string& texturePath,
                   const IlluminationModel illuminationModel,
                   const float specularStrength) :
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

Colour Material::flatShadedColour(
    const Transformation& camera,
    const Transformation& light,
    const glm::vec3& point,
    const glm::vec3& normal,
    const Colour& ambientColour,
    const Colour& diffuseColour,
    const bool inShadow)
{

    const auto lightDisplacement = (light.position - camera.position) * camera.rotation - point;
    const auto normalisedLightDisplacement = glm::normalize(lightDisplacement);
    const auto distanceFromLight = glm::length(lightDisplacement);

    if (inShadow)
    {
        return getAmbient(0.2f, ambientColour);
    }

    return getAmbient(0.2f, ambientColour)
        + getDiffuse(normalisedLightDisplacement, distanceFromLight, normal, diffuseColour) * light.scale;
}

Colour Material::phongShadedColour(
    const Transformation& camera,
    const Transformation& light,
    const glm::vec3& point,
    const glm::vec3& normal,
    const float specularStrength,
    const Colour& ambientColour,
    const Colour& diffuseColour,
    const Colour& specularColour,
    const bool inShadow)
{
    const auto lightDisplacement = (light.position - camera.position) * camera.rotation - point;
    const auto normalisedLightDisplacement = glm::normalize(lightDisplacement);
    const auto distanceFromLight = glm::length(lightDisplacement);

    if (inShadow)
    {
        return getAmbient(0.2f, ambientColour);
    }

    return getAmbient(0.2f, ambientColour) +
    (getDiffuse(normalisedLightDisplacement, distanceFromLight, normal, diffuseColour)
        + getPhong(normalisedLightDisplacement, point, normal, specularColour, specularStrength)) * light.scale;
}

Colour Material::getAmbient(
    const float ambientStrength,
    const Colour& ambientColour)
{
    return ambientColour * ambientStrength;
}

Colour Material::getDiffuse(
    const glm::vec3& normalisedLightDisplacement,
    const float distanceFromLight,
    const glm::vec3& normal,
    const Colour& diffuseColour)
{
    return diffuseColour * glm::clamp(2.f * (glm::dot(normal, normalisedLightDisplacement)) / distanceFromLight,
                                      0.0f, 1.0f);
}

Colour Material::getPhong(
    const glm::vec3& normalisedLightDisplacement,
    const glm::vec3& point,
    const glm::vec3& normal,
    const Colour& specularColour,
    const float specularStrength)
{
    const auto reflectedDirection = normalisedLightDisplacement
        - 2.0f * normal * glm::dot(normalisedLightDisplacement, normal);


    return specularColour * (1.f - glm::pow(0.8f, specularStrength / 2.f)) * glm::pow(
        glm::max(glm::dot(glm::normalize(point), glm::normalize(reflectedDirection)), 0.0f), specularStrength);
}
