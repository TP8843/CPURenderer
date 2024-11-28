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
    const float correctedLuminance = luminance / (1.f + luminance);

    glm::vec4 correctedColour = glm::clamp(correctedLuminance * (colour / luminance), 0.f, 1.f);

    correctedColour = glm::vec4(
        glm::pow(correctedColour.r, 1.f / 2.2f),
        glm::pow(correctedColour.g, 1.f / 2.2f),
        glm::pow(correctedColour.b, 1.f / 2.2f),
        glm::pow(correctedColour.a, 1.f / 2.2f));

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

bool Material::hasNormalMap() const
{
    return hasNormalBool;
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
        glm::pow(static_cast<float>(texture[startingPosition + 0]) / 255.0f, 2.2f),
        glm::pow(static_cast<float>(texture[startingPosition + 1]) / 255.0f, 2.2f),
        glm::pow(static_cast<float>(texture[startingPosition + 2]) / 255.0f, 2.2f),
        1
    };
}

glm::vec3 Material::getNormal(const int x, const int y) const
{
    int tiledX = x % textureWidth;
    int tiledY = y % textureHeight;

    if (tiledX < 0) tiledX += textureWidth;
    if (tiledY < 0) tiledY += textureHeight;

    if (tiledX < 0 || tiledX >= textureWidth || tiledY < 0 || tiledY >= textureHeight)
    {
        std::cout << "Normal out of bounds at (" << tiledX << "," << tiledY << ")" << std::endl;
        return {0, 0, 1,};
    }

    const int startingPosition = (textureWidth * tiledY + tiledX) * charsPerPixel;

    const auto normalVector = glm::vec3{
        (static_cast<float>(normal[startingPosition + 0]) / 255.0f) * 2.f - 1.f,
        (static_cast<float>(normal[startingPosition + 1]) / 255.0f) * 2.f - 1.f,
        (static_cast<float>(normal[startingPosition + 2]) / 127.0f - 1.f)
    };

    return glm::normalize(normalVector);
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
    const float shadowProportion) const
{
    const glm::vec4 textureColour = getPixelTextureColour(
        glm::round(texturePosition.x),
        glm::round(texturePosition.y));

    switch (illuminationModel)
    {
    case MIRROR:
    case PHONG: return phongShadedColour(
            camera,
            light,
            point,
            normal,
            specularStrength,
            textureColour * 0.2f,
            textureColour,
            glm::vec4(1),
            shadowProportion);

    default: return flatShadedColour(
            camera,
            light,
            point,
            normal,
            textureColour * 0.2f,
            textureColour,
            shadowProportion);
    }
}

glm::vec4 Material::getColourAtPointInCameraSpace(
    const Transformation& camera,
    const Transformation& light,
    const glm::vec3& point,
    const glm::vec3& normal,
    const IlluminationModel& illuminationModel,
    const float shadowProportion) const
{
    switch (illuminationModel)
    {
    case MIRROR:
    case PHONG: return phongShadedColour(
            camera,
            light,
            point,
            normal,
            specularStrength,
            colour * 0.2f,
            colour,
            glm::vec4(1),
            shadowProportion);

    default: return flatShadedColour(
            camera,
            light,
            point,
            normal,
            colour * 0.2f,
            colour,
            shadowProportion);
    }
}

Material::Material() = default;

Material::Material(glm::vec4 colour,
                   const IlluminationModel illuminationModel,
                   const float specularStrength) : colour(std::move(colour)),
                                                   illuminationModel(illuminationModel),
                                                   specularStrength(specularStrength),
hasTextureBool(false),
hasNormalBool(false)
{
}

Material::Material(const glm::vec4& colour,
                   const std::string& texturePath,
                   const IlluminationModel illuminationModel,
                   const float specularStrength) :
    colour(colour),
    illuminationModel(illuminationModel),
    specularStrength(specularStrength),
    hasTextureBool(true),
    hasNormalBool(false)
{
    std::cout << "Attempting to load texture from " << texturePath << std::endl;

    texture = stbi_load(texturePath.c_str(),
                        &textureWidth,
                        &textureHeight,
                        nullptr,
                        STBI_rgb_alpha);
}

Material::Material(
    const glm::vec4& colour,
    const std::string& texturePath,
    const IlluminationModel illuminationModel,
    const std::string& normalPath,
    const float specularStrength) :
    colour(colour),
    illuminationModel(illuminationModel),
    specularStrength(specularStrength),
    hasTextureBool(true),
    hasNormalBool(true)
{
    std::cout << "Attempting to load diffuse texture from " << texturePath << std::endl;
    std::cout << "Attempting to load normal texture from " << normalPath << std::endl;

    normal = stbi_load(normalPath.c_str(),
                       &textureHeight,
                       &textureHeight,
                       nullptr,
                       STBI_rgb_alpha);

    texture = stbi_load(texturePath.c_str(),
                        &textureWidth,
                        &textureHeight,
                        nullptr,
                        STBI_rgb_alpha);
}

Material::Material(
    const glm::vec4& colour,
    const IlluminationModel illuminationModel,
    const std::string& normalPath,
    const float specularStrength) :
    colour(colour),
    illuminationModel(illuminationModel),
    specularStrength(specularStrength),
    hasNormalBool(true)
{
    std::cout << "Attempting to load normal texture from " << normalPath << std::endl;

    normal = stbi_load(normalPath.c_str(),
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
    const float shadowProportion)
{
    const auto lightDisplacement = (light.position - camera.position) * camera.rotation - point;
    const auto normalisedLightDisplacement = glm::normalize(lightDisplacement);
    const auto distanceFromLight = glm::length(lightDisplacement);

    return getAmbient(0.05f, ambientColour)
        + (1.f - shadowProportion) * getDiffuse(normalisedLightDisplacement, distanceFromLight, normal, diffuseColour) *
        light.scale;
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
    const float shadowProportion)
{
    const auto lightDisplacement = (light.position - camera.position) * camera.rotation - point;
    const auto normalisedLightDisplacement = glm::normalize(lightDisplacement);
    const auto distanceFromLight = glm::length(lightDisplacement);

    return getAmbient(0.05f, ambientColour) +
        (1.f - shadowProportion) * ((getDiffuse(normalisedLightDisplacement, distanceFromLight, normal, diffuseColour)
            + getSpecular(normalisedLightDisplacement, point, normal, specularColour, specularStrength)) * light.scale);
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
    return diffuseColour * glm::max((glm::dot(normal, normalisedLightDisplacement))
                                    / (1.f * glm::pow(distanceFromLight, 2.f)),
                                    0.0f);
}

glm::vec4 Material::getSpecular(
    const glm::vec3& normalisedLightDisplacement,
    const glm::vec3& point,
    const glm::vec3& normal,
    const glm::vec4& specularColour,
    const float specularStrength)
{
    const auto reflectedDirection = normalisedLightDisplacement
        - 2.0f * normal * glm::dot(normalisedLightDisplacement, normal);

    return specularColour * glm::pow(glm::max(
                                         glm::dot(glm::normalize(point), glm::normalize(reflectedDirection)),
                                         0.0f), specularStrength);
}
