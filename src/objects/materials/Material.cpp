#include "Material.h"
#define STB_IMAGE_IMPLEMENTATION
#include <TexturePoint.h>

#include "../Transformation.h"
#include "../../libs/stb_image.h"

glm::vec4 Material::getColour() const
{
    return colour;
}

uint32_t Material::getScreenColour(const glm::vec4 colour)
{

    const float luminance = 0.2126 * colour.r + 0.7152 * colour.g + 0.0722 * colour.b;

    const glm::vec4 correctedColour = colour / (1.f + colour);

    const uint32_t a = static_cast<uint32_t>(glm::round(glm::max(correctedColour.a * 255.f, 0.f))) << 24;
    const uint32_t r = static_cast<uint32_t>(glm::round(glm::max(correctedColour.r * 255.f, 0.f))) << 16;
    const uint32_t g = static_cast<uint32_t>(glm::round(glm::max(correctedColour.g * 255.f, 0.f))) << 8;
    const uint32_t b = static_cast<uint32_t>(glm::round(glm::max(correctedColour.b * 255.f, 0.f)));

    return a | r | g | b;
}

IlluminationModel Material::getIlluminationModel() const
{
    return illuminationModel;
}

bool Material::hasTexture() const
{
    return hasTextureBool;
}

glm::vec4 Material::getPixelTextureColour(const int x, const int y) const
{
    int tiledX = x % textureWidth;
    int tiledY = y % textureHeight;

    if (tiledX < 0) tiledX += textureWidth;
    if (tiledY < 0) tiledY += textureHeight;

    if (tiledX < 0 || tiledX >= textureWidth || tiledY < 0 || tiledY >= textureHeight)
    {
        std::cout << "Texture out of bounds at (" << tiledX << "," << tiledY << ")" << std::endl;
        return {0, 0, 0, 1};
    }

    const int startingPosition = (textureWidth * tiledY + tiledX) * charsPerPixel;

    return {
        static_cast<float>(texture[startingPosition + 0]) / 255.0f,
        static_cast<float>(texture[startingPosition + 1]) / 255.0f,
        static_cast<float>(texture[startingPosition + 2]) / 255.0f,
        1
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

glm::vec4 Material::getColourAtPointInCameraSpace(
    const Transformation& camera,
    const Transformation& light,
    const glm::vec3& point,
    const glm::vec3& normal,
    const TexturePoint& texturePosition,
    const IlluminationModel& illuminationModel,
    const bool inShadow) const
{
    const glm::vec4 textureColour = getPixelTextureColour(
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
            glm::vec4(1),
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

glm::vec4 Material::getColourAtPointInCameraSpace(
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
            glm::vec4(1),
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

Material::Material(glm::vec4 colour,
                   const IlluminationModel illuminationModel,
                   const float specularStrength) : colour(std::move(colour)),
                                                   illuminationModel(illuminationModel),
                                                   specularStrength(specularStrength)
{
}

Material::Material(const glm::vec4& colour,
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

glm::vec4 Material::flatShadedColour(
    const Transformation& camera,
    const Transformation& light,
    const glm::vec3& point,
    const glm::vec3& normal,
    const glm::vec4& ambientColour,
    const glm::vec4& diffuseColour,
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

glm::vec4 Material::phongShadedColour(
    const Transformation& camera,
    const Transformation& light,
    const glm::vec3& point,
    const glm::vec3& normal,
    const float specularStrength,
    const glm::vec4& ambientColour,
    const glm::vec4& diffuseColour,
    const glm::vec4& specularColour,
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

glm::vec4 Material::getAmbient(
    const float ambientStrength,
    const glm::vec4& ambientColour)
{
    return ambientColour * ambientStrength;
}

glm::vec4 Material::getDiffuse(
    const glm::vec3& normalisedLightDisplacement,
    const float distanceFromLight,
    const glm::vec3& normal,
    const glm::vec4& diffuseColour)
{
    return diffuseColour * glm::max((glm::dot(normal, normalisedLightDisplacement)) / (1.f * distanceFromLight),
                                      0.0f);
}

glm::vec4 Material::getPhong(
    const glm::vec3& normalisedLightDisplacement,
    const glm::vec3& point,
    const glm::vec3& normal,
    const glm::vec4& specularColour,
    const float specularStrength)
{
    const auto reflectedDirection = normalisedLightDisplacement
        - 2.0f * normal * glm::dot(normalisedLightDisplacement, normal);

    return specularColour * glm::pow(
        glm::max(glm::dot(glm::normalize(point), glm::normalize(reflectedDirection)), 0.0f), specularStrength);
}
