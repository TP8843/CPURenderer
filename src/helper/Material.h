//
// Created by tparr on 28/10/24.
//

#ifndef MATERIAL_H
#define MATERIAL_H
#include <Colour.h>
#include <string>
#include <TextureMap.h>

class Material {
public:
    Colour getColour() const;
    bool hasTexture() const;
    uint32_t getPixelTextureColour(size_t x, size_t y) const;
    size_t getTextureWidth() const;
    size_t getTextureHeight() const;

    Material();
    Material(Colour colour);
    Material(Colour colour, TextureMap texture);

private:
    Colour colour;

    TextureMap texture;
    bool hasTextureBool{};
};



#endif //MATERIAL_H
