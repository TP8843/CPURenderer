#include "MaterialMap.h"

void MaterialMap::addMaterial(const std::string& name, const Material& material)
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

MaterialMap MaterialMap::import(MaterialMap& materialMap, const std::string &filePath, const std::string &folderPath)
{
    std::ifstream MaterialFile(filePath);
    std::string line;
    std::string currentMaterialName = defaultName();

    bool materialToStore = false;
    bool hasTexture = false;
    bool hasColour = false;

    std::string currentTextureFilename;
    Colour currentColour;

    IlluminationModel illuminationModel = defaultIlluminationModel;
    float shininess = defaultShininess;

    while (getline(MaterialFile, line)) {
        // Remove Windows carriage return. From https://stackoverflow.com/questions/2528995/remove-r-from-a-string-in-c
        if (!line.empty() && line[line.size() - 1] == '\r')
            line.erase(line.size() - 1);

        const auto tokens = split(line, ' ');

        // Defines a new material
        if (tokens.at(0) == "newmtl" && !tokens.at(1).empty()) {
            if (materialToStore) {
                if (hasTexture && hasColour) {
                    materialMap.addMaterial(currentMaterialName,
                                            Material(currentColour, illuminationModel, shininess,
                                                     folderPath + currentTextureFilename));
                } else if (hasTexture) {
                    materialMap.addMaterial(currentMaterialName,
                                            Material(Colour(255, 255, 255), illuminationModel, shininess,
                                                     folderPath + currentTextureFilename));
                } else if (hasColour) {
                    materialMap.addMaterial(currentMaterialName,
                                            Material(currentColour, illuminationModel, shininess));
                }
            }

            materialToStore = false;
            hasColour = false;
            hasTexture = false;

            currentMaterialName = tokens.at(1);
            illuminationModel = FLAT;
            shininess = 32.0f;
        }

        // Colour for diffuse lighting
        if (tokens.at(0) == "Kd" && !tokens.at(1).empty()) {
            materialToStore = true;

            const auto colourValue = Colour(
                static_cast<int>(glm::round(std::stof(tokens.at(1)) * 255.0f)),
                static_cast<int>(glm::round(std::stof(tokens.at(2)) * 255.0f)),
                static_cast<int>(glm::round(std::stof(tokens.at(3)) * 255.0f)));

            hasColour = true;
            currentColour = colourValue;
        }

        // Strength of specular highlights
        if (tokens.at(0) == "Ns" && !tokens.at(1).empty()) {
            shininess = std::stof(tokens.at(1));
        }

        // Location of texture map
        if (tokens.at(0) == "map_Kd" && !tokens.at(1).empty()) {
            materialToStore = true;

            currentTextureFilename = tokens.at(1);

            const auto &texturePath = tokens.at(1);

            hasTexture = true;
        }

        // Illumination type (UNSHADED, FLAT, PHONG)
        if (tokens.at(0) == "illum" && !tokens.at(1).empty()) {
            switch (std::stoi(tokens.at(1))) {
                case 0: illuminationModel = UNSHADED;
                case 1: illuminationModel = FLAT;
                case 2: illuminationModel = PHONG;
                default: illuminationModel = FLAT;
            }
        }
    }

    if (materialToStore) {
        if (hasTexture && hasColour) {
            materialMap.addMaterial(currentMaterialName,
                                    Material(currentColour, illuminationModel, shininess,
                                             folderPath + currentTextureFilename));
        } else if (hasTexture) {
            materialMap.addMaterial(currentMaterialName,
                                    Material(Colour(255, 255, 255), illuminationModel, shininess,
                                             folderPath + currentTextureFilename));
        } else if (hasColour) {
            materialMap.addMaterial(currentMaterialName, Material(currentColour, illuminationModel, shininess));
        }
    }

    return materialMap;
}
