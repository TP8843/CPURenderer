//
// Created by tparr on 28/10/24.
//

#ifndef MATERIAL_H
#define MATERIAL_H
#include <Colour.h>
#include <string>
#include <TextureMap.h>

struct Material {
    std::string name{};
    Colour colour{};

    TextureMap textureMap{};
    bool hasTexture{};

    Material();
    Material(std::string name, Colour colour);
    Material(std::string name, Colour colour, TextureMap textureMap);
};



#endif //MATERIAL_H
