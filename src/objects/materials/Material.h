#ifndef MATERIAL_H
#define MATERIAL_H

#include <Colour.h>
#include <TextureMap.h>
#include <TexturePoint.h>

#include "IlluminationModel.h"
#include "../Transformation.h"

class Material
{
public:
    Colour getColour() const;
    IlluminationModel getIlluminationModel() const;
    bool hasTexture() const;
    Colour getPixelTextureColour(int x, int y) const;
    size_t getTextureWidth() const;
    size_t getTextureHeight() const;

    Colour getColourAtPointInCameraSpace(const ::Transformation& camera,
                                         const ::Transformation& light,
                                         const glm::vec3& point,
                                         const glm::vec3& normal,
                                         const TexturePoint& texturePosition,
                                         const IlluminationModel& illuminationModel,
                                         bool inShadow = false) const;

    Colour getColourAtPointInCameraSpace(const Transformation& camera,
                                         const Transformation& light,
                                         const glm::vec3& point,
                                         const glm::vec3& normal,
                                         const IlluminationModel& illuminationModel,
                                         bool inShadow = false) const;

    Material();
    explicit Material(Colour colour, IlluminationModel illuminationModel = FLAT, float specularStrength = 0.f);
    explicit Material(const Colour& colour, const std::string& texturePath, IlluminationModel illuminationModel = FLAT,
                      float specularStrength = 0.f);

private:
    static constexpr int charsPerPixel = 4;

    static Colour flatShadedColour(
        const Transformation& camera,
        const Transformation& light,
        const glm::vec3& point,
        const glm::vec3& normal,
        const Colour& ambientColour,
        const Colour& diffuseColour,
        bool inShadow = false);

    static Colour phongShadedColour(
        const Transformation& camera,
        const Transformation& light,
        const glm::vec3& point,
        const glm::vec3& normal,
        float specularStrength,
        const Colour& ambientColour,
        const Colour& diffuseColour,
        const Colour& specularColour,
        bool inShadow = false);

    static Colour getAmbient(
        float ambientStrength,
        const Colour& ambientColour);

    static Colour getDiffuse(
        const glm::vec3& normalisedLightDisplacement,
        float distanceFromLight,
        const glm::vec3& normal, const Colour& diffuseColour);

    static Colour getPhong(
        const glm::vec3& normalisedLightDisplacement,
        const glm::vec3& point,
        const glm::vec3& normal,
        const Colour& specularColour,
        float specularStrength);

    Colour colour;
    IlluminationModel illuminationModel = FLAT;
    float specularStrength = 1.0f;

    int textureWidth = 0;
    int textureHeight = 0;
    unsigned char* texture{};
    bool hasTextureBool{};
};

#endif //MATERIAL_H
