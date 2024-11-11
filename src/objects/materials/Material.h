#ifndef MATERIAL_H
#define MATERIAL_H

#include <Colour.h>
#include <TextureMap.h>

#include "IlluminationModel.h"
#include "../Camera.h"
#include "../Light.h"

class Material {
public:
    Colour getColour() const;
    IlluminationModel getIlluminationModel() const;
    bool hasTexture() const;
    Colour getPixelTextureColour(size_t x, size_t y) const;
    size_t getTextureWidth() const;
    size_t getTextureHeight() const;

    float getColourAtPointInCameraSpace(const Camera& camera,
                           const Light &light,
                           const glm::vec3 &point,
                           const glm::vec3 &normal) const;

    Material();
    Material(Colour colour, IlluminationModel illuminationModel, float specularStrength);
    Material(const Colour& colour, IlluminationModel illuminationModel, float specularStrength, const std::string& texturePath);

private:
    static constexpr int charsPerPixel = 4;

    Colour colour;
    IlluminationModel illuminationModel = FLAT;
    float specularStrength = 1.0f;

    int textureWidth = 0;
    int textureHeight = 0;
    unsigned char* texture;
    bool hasTextureBool{};
};

#endif //MATERIAL_H
