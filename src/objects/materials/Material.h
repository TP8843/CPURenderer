#ifndef MATERIAL_H
#define MATERIAL_H

#include <Colour.h>
#include <TextureMap.h>
#include <TexturePoint.h>

#include "IlluminationModel.h"
#include "../Transformation.h"

class Material {
public:
    Colour getColour() const;
    IlluminationModel getIlluminationModel() const;
    bool hasTexture() const;
    Colour getPixelTextureColour(int x, int y) const;
    size_t getTextureWidth() const;
    size_t getTextureHeight() const;

    Colour getColourAtPointInCameraSpace(const ::Transformation &camera,
                                         const ::Transformation &light,
                                         const glm::vec3 & point,
                                         const glm::vec3 & normal,
                                         const TexturePoint &texturePosition,
                                         bool inShadow = false) const;

    Colour getColourAtPointInCameraSpace(const Transformation &camera,
                                           const Transformation &light,
                                           const glm::vec3 &point,
                                           const glm::vec3 &normal,
                                           bool inShadow = false) const;

    Material();
    Material(Colour colour, IlluminationModel illuminationModel, float specularStrength);
    Material(const Colour& colour, IlluminationModel illuminationModel, float specularStrength, const std::string& texturePath);

private:
    static constexpr int charsPerPixel = 4;

    static Colour colourAtPointInCameraSpace(const ::Transformation &camera,
                                             const ::Transformation &light,
                                             const glm::vec3 & point,
                                             const glm::vec3 & normal,
                                             float specularStrength,
                                             const Colour &ambientColour,
                                             const Colour &diffuseColour,
                                             const Colour &specularColour,
                                             const bool inShadow = false);

    Colour colour;
    IlluminationModel illuminationModel = FLAT;
    float specularStrength = 1.0f;

    int textureWidth = 0;
    int textureHeight = 0;
    unsigned char* texture{};
    bool hasTextureBool{};
};

#endif //MATERIAL_H
