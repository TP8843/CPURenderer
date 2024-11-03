#include "Material.h"

Colour Material::getColour() const
{
        return colour;
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

Material::Material(Colour colour):
        colour(std::move(colour))
{}

Material::Material(Colour colour, TextureMap texture) :
        colour(std::move(colour)), texture(std::move(texture)), hasTextureBool(true)
{}
