//
// Created by tparr on 28/10/24.
//

#include "Material.h"

#include <utility>

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

        return Colour(
                rawColour & (0xFF),
                rawColour & (0xFF),
                rawColour & (0xFF)
        );
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
        colour(std::move(colour)), hasTextureBool(true), texture(std::move(texture))
{}
