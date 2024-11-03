#ifndef POINTDATA_H
#define POINTDATA_H

#include <DrawingWindow.h>
#include <TexturePoint.h>
#include <glm/glm.hpp>

#include "../helper/Material.h"
#include "../objects/Light.h"

namespace FragmentData
{
    struct FilledData
    {
        glm::vec3 proportion;
        float depth;
        glm::vec3 position3D;

        FilledData(glm::vec3 proportion, float depth, glm::vec3 position3D);

        FilledData operator+(const FilledData& other) const;
        FilledData operator-(const FilledData& other) const;

        template <typename N>
        FilledData operator*(const N num)
        {
            return FilledData(proportion * num, depth * num, position3D * num);
        }
    };

    struct TextureData
    {
        TexturePoint texturePoint;
        glm::vec3 proportion;
        float depth;
        glm::vec3 position3D;

        TextureData(TexturePoint texturePoint,
                    glm::vec3 proportion,
                    float depth, glm::vec3
                    position3D);

        TextureData operator+(const TextureData& other) const;
        TextureData operator-(const TextureData& other) const;

        template <typename N>
        TextureData operator*(const N num)
        {
            return TextureData(texturePoint * num,
                               proportion * num,
                               depth * num,
                               position3D * num);
        }
    };

    struct DataUniform
    {
        DrawingWindow& window;
        float** depthBuffer;
        Material& material;
        Camera& camera;
        Light& light;
        glm::vec3 normal;

        explicit DataUniform(DrawingWindow& window,
                             float** depthBuffer,
                             Material& material,
                             Camera& camera,
                             Light& light,
                             const glm::vec3& normal);
    };
}

#endif //POINTDATA_H
