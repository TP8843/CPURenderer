#include "Material.h"

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

Colour Material::getPixelTextureColour(const size_t x, const size_t y) const
{
        const uint32_t rawColour = texture.pixels[y * texture.width + x];

        return {
                static_cast<int>(rawColour & 0xFF),
                static_cast<int>(rawColour >> 8 & 0xFF),
                static_cast<int>(rawColour >> 16 & 0xFF)
        };
}

size_t Material::getTextureWidth() const
{
        return texture.width;
}

size_t Material::getTextureHeight() const
{
        return texture.height;
}

Material::Material() = default;

Material::Material(Colour colour, const IlluminationModel illuminationModel, float specularStrength):
        colour(std::move(colour)), illuminationModel(illuminationModel), specularStrength(specularStrength)
{}

Material::Material(Colour colour, const IlluminationModel illuminationModel, float specularStrength, TextureMap texture) :
        colour(std::move(colour)), illuminationModel(illuminationModel), specularStrength(specularStrength), texture(std::move(texture)), hasTextureBool(true)
{}
