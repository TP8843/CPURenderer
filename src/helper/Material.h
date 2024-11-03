#ifndef MATERIAL_H
#define MATERIAL_H

#include <Colour.h>
#include <TextureMap.h>

class Material {
public:
    Colour getColour() const;
    bool hasTexture() const;
    Colour getPixelTextureColour(size_t x, size_t y) const;
    size_t getTextureWidth() const;
    size_t getTextureHeight() const;

    Material();
    explicit Material(Colour colour);
    Material(Colour colour, TextureMap texture);

private:
    Colour colour;

    TextureMap texture;
    bool hasTextureBool{};
};



#endif //MATERIAL_H
