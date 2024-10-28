//
// Created by tparr on 28/10/24.
//

#include "Material.h"

#include <utility>

Material::Material() = default;

Material::Material(std::string name, Colour colour):
        name(std::move(name)), colour(std::move(colour)), hasTexture(false)
{}

Material::Material(std::string name, Colour colour, TextureMap textureMap) :
        name(std::move(name)), colour(std::move(colour)), hasTexture(true), textureMap(std::move(textureMap))
{}
