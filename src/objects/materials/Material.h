#ifndef MATERIAL_H
#define MATERIAL_H

#include <TextureMap.h>
#include <TexturePoint.h>

#include "IlluminationModel.h"
#include "../Transformation.h"

class Material
{
public:
    glm::vec4 getColour() const;
    static uint32_t getScreenColour(glm::vec4 colour);

    IlluminationModel getIlluminationModel() const;
    bool hasTexture() const;
    glm::vec4 getPixelTextureColour(int x, int y) const;
    size_t getTextureWidth() const;
    size_t getTextureHeight() const;

    glm::vec4 getColourAtPointInCameraSpace(const ::Transformation& camera,
                                            const ::Transformation& light,
                                            const glm::vec3& point,
                                            const glm::vec3& normal,
                                            const TexturePoint& texturePosition,
                                            const IlluminationModel& illuminationModel,
                                            float shadowProportion = 0.f) const;

    glm::vec4 getColourAtPointInCameraSpace(const ::Transformation& camera,
                                            const ::Transformation& light,
                                            const glm::vec3& point,
                                            const glm::vec3& normal,
                                            const IlluminationModel& illuminationModel,
                                            float shadowProportion = false) const;

    Material();
    explicit Material(glm::vec4 colour, IlluminationModel illuminationModel = FLAT, float specularStrength = 0.f);
    explicit Material(const glm::vec4& colour, const std::string& texturePath, IlluminationModel illuminationModel = FLAT,
                      float specularStrength = 0.f);

private:
    static constexpr int charsPerPixel = 4;

    static glm::vec4 flatShadedColour(
        const Transformation& camera,
        const Transformation& light,
        const glm::vec3& point,
        const glm::vec3& normal,
        const glm::vec4& ambientColour,
        const glm::vec4& diffuseColour, float shadowProportion);

    static glm::vec4 phongShadedColour(
        const Transformation& camera,
        const Transformation& light,
        const glm::vec3& point,
        const glm::vec3& normal,
        float specularStrength,
        const glm::vec4& ambientColour,
        const glm::vec4& diffuseColour,
        const glm::vec4& specularColour, float shadowProportion);

    static glm::vec4 getAmbient(
        float ambientStrength,
        const glm::vec4& ambientColour);

    static glm::vec4 getDiffuse(
        const glm::vec3& normalisedLightDisplacement,
        float distanceFromLight,
        const glm::vec3& normal, const glm::vec4& diffuseColour);

    static glm::vec4 getPhong(
        const glm::vec3& normalisedLightDisplacement,
        const glm::vec3& point,
        const glm::vec3& normal,
        const glm::vec4& specularColour,
        float specularStrength);

    glm::vec4 colour;
    IlluminationModel illuminationModel = FLAT;
    float specularStrength = 1.f;
    float refractiveIndex = 1.f;

    int textureWidth = 0;
    int textureHeight = 0;
    unsigned char* texture{};
    bool hasTextureBool{};
};

#endif //MATERIAL_H
