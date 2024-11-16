#ifndef MATERIALMAP_H
#define MATERIALMAP_H
#include <unordered_map>

#include "Material.h"


class MaterialMap {
public:
    void addMaterial(const std::string& name, const Material& material);
    Material& getMaterial(const std::string& name);

    MaterialMap();

    static constexpr float defaultShininess = 256.0f;
    static std::string defaultName() { return "Backup"; }
    static constexpr IlluminationModel defaultIlluminationModel = FLAT;

    static MaterialMap import(MaterialMap& materialMap, const std::string &folderPath, const std::string &file);

private:
    std::unordered_map<std::string, Material> materials;
};



#endif //MATERIALMAP_H
