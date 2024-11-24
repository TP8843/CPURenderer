#ifndef MATERIALMAP_H
#define MATERIALMAP_H
#include <unordered_map>

#include "Material.h"


class MaterialMap {
public:
    void addMaterial(const std::string& name, const Material& material);
    Material& getMaterial(const std::string& name);
    int size() const;
    bool hasMaterial(const std::string& name) const;

    glm::vec3 getSampledVec3(unsigned int x, unsigned int y, unsigned int sample) const;
    glm::vec2 getSampledVec2(unsigned int x, unsigned int y, unsigned int sample) const;


    MaterialMap();
    ~MaterialMap();

    static constexpr float defaultShininess = 8.0f;
    static std::string defaultName() { return "Backup"; }
    static std::string defaultSampleNoise() { return "../noise/sample_noise.png"; }
    static std::string defaultScreenNoise() { return "../noise/screen_noise.png"; }
    static constexpr IlluminationModel defaultIlluminationModel = FLAT;

    static MaterialMap import(MaterialMap& materialMap, const std::string& originalPath, const std::string& folderPath, const std::string& file);

private:
    std::unordered_map<std::string, Material> materials;

    unsigned char* screenNoise;
    int screenNoiseHeight;
    int screenNoiseWidth;

    unsigned char* sampleNoise;
    int sampleNoiseHeight;
    int sampleNoiseWidth;
};



#endif //MATERIALMAP_H
