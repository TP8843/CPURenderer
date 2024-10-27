#ifndef MATERIAL_H
#define MATERIAL_H
#include <Colour.h>
#include <TextureMap.h>

struct Material
{
    Colour colour;
    TextureMap texture;

    explicit Material(const Colour& colour);
    Material(const Colour& colour, const TextureMap& texture);
};

#endif //MATERIAL_H
