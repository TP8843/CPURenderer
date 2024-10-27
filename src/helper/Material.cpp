#include "Material.h"

Material::Material(const Colour& colour) : colour(colour)
{}

Material::Material(const Colour& colour, const TextureMap& texture) : colour(colour), texture(texture)
{}

