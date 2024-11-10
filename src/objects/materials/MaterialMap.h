//
// Created by Thomas Parr on 30/10/2024.
//

#ifndef MATERIALMAP_H
#define MATERIALMAP_H
#include <unordered_map>

#include "Material.h"


class MaterialMap {
public:
    void addMaterial(std::string name, Material material);
    Material& getMaterial(const std::string& name);

    MaterialMap();

private:
    std::unordered_map<std::string, Material> materials;
};



#endif //MATERIALMAP_H
