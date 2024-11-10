//
// Created by Thomas Parr on 30/10/2024.
//

#include "MaterialMap.h"

void MaterialMap::addMaterial(std::string name, Material material)
{
    materials.emplace(name, material);
}

Material& MaterialMap::getMaterial(const std::string& name)
{
    return materials.at(name);
}

MaterialMap::MaterialMap() :
    materials(std::unordered_map<std::string, Material>())
{}
