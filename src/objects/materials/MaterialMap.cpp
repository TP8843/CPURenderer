#include "MaterialMap.h"

#include "../../libs/stb_image.h"
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

// Ideas from https://blog.demofox.org/2020/05/16/using-blue-noise-for-raytraced-soft-shadows/
glm::vec3 MaterialMap::getSampledVec3(const unsigned int x, const unsigned int y, const unsigned int sample) const
{
    const int angle = 2.f * 3.1415926535f * screenNoise[((y * screenNoiseWidth + x) * 3) % (screenNoiseWidth *
        screenNoiseHeight)];
    const int sampleStart = (sample * 3) % (sampleNoiseWidth * sampleNoiseHeight);

    auto result = glm::vec3(
        (sampleNoise[sampleStart + 0] / 255.f) * glm::sin(angle),
        (sampleNoise[sampleStart + 1] / 255.f) * glm::sin(angle),
        (sampleNoise[sampleStart + 2] / 255.f) * glm::sin(angle));

    return result;
}

glm::vec2 MaterialMap::getSampledVec2(unsigned int x, unsigned int y, unsigned int sample) const
{
    const int angle1 = 2.f * 3.1415926535f * screenNoise[((y * screenNoiseWidth + x) * 3) % (screenNoiseWidth *
        screenNoiseHeight)];
    const int angle2 = 2.f * 3.1415926535f * screenNoise[((y * screenNoiseWidth + x) * 3 + 1) % (screenNoiseWidth *
        screenNoiseHeight)];
    const int sampleStart = (sample * 3) % (sampleNoiseWidth * sampleNoiseHeight);

    const auto result = glm::vec2(
        (sampleNoise[sampleStart + 0] / 127.5f - 1.f) * glm::sin(angle1),
        (sampleNoise[sampleStart + 1] / 127.5f - 1.f) * glm::sin(angle2));

    return result;
}

MaterialMap::MaterialMap() :
    materials(std::unordered_map<std::string, Material>())
{
    sampleNoise = stbi_load(defaultSampleNoise().c_str(),
                            &sampleNoiseWidth,
                            &sampleNoiseHeight,
                            nullptr,
                            STBI_rgb);

    screenNoise = stbi_load(defaultScreenNoise().c_str(),
                            &screenNoiseWidth,
                            &screenNoiseHeight,
                            nullptr,
                            STBI_rgb);
}

MaterialMap::~MaterialMap()
{
    // stbi_image_free(sampleNoise);
    // stbi_image_free(screenNoise);
}

MaterialMap MaterialMap::import(MaterialMap& materialMap, const std::string& originalPath,
                                const std::string& folderPath, const std::string& file)
{
    std::ifstream MaterialFile(StringHelpers::concatFolderFile(folderPath, file));
    std::string line;
    std::string currentMaterialName = defaultName();

    bool materialToStore = false;
    bool hasTexture = false;
    bool hasNormal = false;
    bool hasSpecular = false;

    std::string currentTextureFilename;
    std::string currentNormalFilename;
    std::string currentSpecularFilename;
    glm::vec4 currentColour = glm::vec4(1);

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
                if (hasTexture && hasNormal && hasSpecular)
                {
                    materialMap.addMaterial(originalPath + currentMaterialName,
                        Material(currentColour,
                                 StringHelpers::concatFolderFile(folderPath, currentTextureFilename),
                                 illuminationModel,
                                 StringHelpers::concatFolderFile(folderPath, currentNormalFilename),
                                 shininess,
                                 StringHelpers::concatFolderFile(folderPath, currentSpecularFilename)));
                }
                else if (hasTexture && hasNormal)
                {
                    materialMap.addMaterial(originalPath + currentMaterialName,
                                            Material(currentColour,
                                                     StringHelpers::concatFolderFile(folderPath, currentTextureFilename),
                                                     illuminationModel,
                                                     StringHelpers::concatFolderFile(folderPath, currentNormalFilename),
                                                     shininess));
                }
                else if (hasNormal && hasSpecular)
                {
                    materialMap.addMaterial(originalPath + currentMaterialName,
                        Material(currentColour,
                                 illuminationModel,
                                 StringHelpers::concatFolderFile(folderPath, currentNormalFilename),
                                 shininess,
                                 StringHelpers::concatFolderFile(folderPath, currentSpecularFilename)));
                }
                else if (hasTexture && hasSpecular)
                {
                    materialMap.addMaterial(originalPath + currentMaterialName,
                                            Material(
                                                currentColour,
                                                StringHelpers::concatFolderFile(folderPath, currentTextureFilename),
                                                illuminationModel,
                                                shininess,
                                                StringHelpers::concatFolderFile(folderPath, currentSpecularFilename)));
                }
                else if (hasNormal)
                {
                    materialMap.addMaterial(originalPath + currentMaterialName,
                        Material(currentColour,
                                 illuminationModel,
                                 StringHelpers::concatFolderFile(folderPath, currentNormalFilename),
                                 shininess));
                }
                else if (hasTexture)
                {
                    materialMap.addMaterial(originalPath + currentMaterialName,
                                            Material(
                                                currentColour,
                                                StringHelpers::concatFolderFile(folderPath, currentTextureFilename),
                                                illuminationModel,
                                                shininess));
                }
                else
                {
                    materialMap.addMaterial(originalPath + currentMaterialName,
                                            Material(currentColour, illuminationModel, shininess));
                }
            }

            materialToStore = false;
            hasTexture = false;
            hasNormal = false;

            currentColour = glm::vec4(1);
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

        // Location of normal map
        if ((tokens.at(0) == "map_Disp" || tokens.at(0) == "bump" || tokens.at(0) == "map_bump") && !tokens.at(1).empty())
        {
            materialToStore = true;

            currentNormalFilename = tokens.at(1);
            hasNormal = true;
        }

        if (tokens.at(0) == "map_Ks" && !tokens.at(1).empty())
        {
            materialToStore = true;

            currentSpecularFilename = tokens.at(1);
            hasSpecular = true;
        }

        // Illumination type (UNSHADED, FLAT, PHONG)
        if (tokens.at(0) == "illum" && !tokens.at(1).empty())
        {
            switch (std::stoi(tokens.at(1)))
            {
            case UNSHADED: illuminationModel = UNSHADED;
                break;
            case FLAT: illuminationModel = FLAT;
                break;
            case PHONG: illuminationModel = PHONG;
                break;
            case MIRROR: illuminationModel = MIRROR;
                break;
            case REFRACTION_NO_FRESNEL: illuminationModel = REFRACTION_NO_FRESNEL;
                break;
            default: illuminationModel = FLAT;
                break;
            }
        }
    }

    if (materialToStore)
    {
        if (hasTexture && hasNormal)
        {
            materialMap.addMaterial(originalPath + currentMaterialName,
                                    Material(currentColour,
                                             StringHelpers::concatFolderFile(folderPath, currentTextureFilename),
                                             illuminationModel,
                                             StringHelpers::concatFolderFile(folderPath, currentNormalFilename),
                                             shininess));
        }
        else if (hasNormal)
        {
            materialMap.addMaterial(originalPath + currentMaterialName,
                Material(currentColour,
                         illuminationModel,
                         StringHelpers::concatFolderFile(folderPath, currentNormalFilename),
                         shininess));
        }
        else if (hasTexture)
        {
            materialMap.addMaterial(originalPath + currentMaterialName,
                                    Material(
                                        currentColour,
                                        StringHelpers::concatFolderFile(folderPath, currentTextureFilename),
                                        illuminationModel,
                                        shininess));
        }
        else
        {
            materialMap.addMaterial(originalPath + currentMaterialName,
                                    Material(currentColour, illuminationModel, shininess));
        }
    }

    return materialMap;
}
