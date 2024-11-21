#include "MaterialMap.h"

#include "../../helper/StringHelpers.h"

void MaterialMap::addMaterial(const std::string& name, const Material& material)
{
    materials.emplace(name, material);
}

Material& MaterialMap::getMaterial(const std::string& name)
{
    return materials.at(name);
}

int MaterialMap::size() const
{
    return materials.size();
}

bool MaterialMap::hasMaterial(const std::string& name) const
{
    return materials.find(name) != materials.end();
}

MaterialMap::MaterialMap() :
    materials(std::unordered_map<std::string, Material>())
{
}

MaterialMap MaterialMap::import(MaterialMap& materialMap, const std::string& originalPath, const std::string& folderPath, const std::string& file)
{
    std::ifstream MaterialFile(StringHelpers::concatFolderFile(folderPath, file));
    std::string line;
    std::string currentMaterialName = defaultName();

    bool materialToStore = false;
    bool hasTexture = false;
    bool hasColour = false;

    std::string currentTextureFilename;
    glm::vec4 currentColour;

    IlluminationModel illuminationModel = defaultIlluminationModel;
    float shininess = defaultShininess;

    while (getline(MaterialFile, line))
    {
        line = StringHelpers::trimLine(line);

        const auto tokens = split(line, ' ');

        // Defines a new material
        if (tokens.at(0) == "newmtl" && !tokens.at(1).empty())
        {
            if (materialToStore)
            {
                if (hasTexture && hasColour)
                {
                    materialMap.addMaterial(originalPath + currentMaterialName,
                                            Material(currentColour,
                                                     StringHelpers::concatFolderFile(
                                                         folderPath, currentTextureFilename),
                                                     illuminationModel,
                                                     shininess));
                }
                else if (hasTexture)
                {
                    materialMap.addMaterial(originalPath + currentMaterialName,
                                            Material(glm::vec4(1),
                                                     StringHelpers::concatFolderFile(
                                                         folderPath, currentTextureFilename),
                                                     illuminationModel,
                                                     shininess));
                }
                else if (hasColour)
                {
                    materialMap.addMaterial(originalPath + currentMaterialName,
                                            Material(currentColour, illuminationModel, shininess));
                }
            }

            materialToStore = false;
            hasColour = false;
            hasTexture = false;

            currentMaterialName = tokens.at(1);
            illuminationModel = FLAT;
            shininess = defaultShininess;
        }

        // Colour for diffuse lighting
        if (tokens.at(0) == "Kd" && !tokens.at(1).empty())
        {
            materialToStore = true;

            const auto colourValue = glm::vec4(
                glm::pow(std::stof(tokens.at(1)), 2.2f),
                glm::pow(std::stof(tokens.at(2)), 2.2f),
                glm::pow(std::stof(tokens.at(3)), 2.2f),
                1);

            hasColour = true;
            currentColour = colourValue;
        }

        // Strength of specular highlights
        if (tokens.at(0) == "Ns" && !tokens.at(1).empty())
        {
            shininess = std::stof(tokens.at(1));
        }

        // Location of texture map
        if (tokens.at(0) == "map_Kd" && !tokens.at(1).empty())
        {
            materialToStore = true;

            currentTextureFilename = tokens.at(1);
            hasTexture = true;
        }

        // Illumination type (UNSHADED, FLAT, PHONG)
        if (tokens.at(0) == "illum" && !tokens.at(1).empty())
        {
            switch (std::stoi(tokens.at(1)))
            {
            case UNSHADED: illuminationModel = UNSHADED; break;
            case FLAT: illuminationModel = FLAT; break;
            case PHONG: illuminationModel = PHONG; break;
            case MIRROR: illuminationModel = MIRROR; break;
            default: illuminationModel = FLAT; break;
            }
        }
    }

    if (materialToStore)
    {
        if (hasTexture && hasColour)
        {
            materialMap.addMaterial(originalPath + currentMaterialName,
                                    Material(
                                        currentColour,
                                        StringHelpers::concatFolderFile(folderPath, currentTextureFilename),
                                        illuminationModel,
                                        shininess));
        }
        else if (hasTexture)
        {
            materialMap.addMaterial(originalPath + currentMaterialName,
                                    Material(
                                        glm::vec4(1),
                                        StringHelpers::concatFolderFile(folderPath, currentTextureFilename),
                                        illuminationModel,
                                        shininess));
        }
        else if (hasColour)
        {
            materialMap.addMaterial(originalPath + currentMaterialName, Material(currentColour, illuminationModel, shininess));
        }
    }

    return materialMap;
}
