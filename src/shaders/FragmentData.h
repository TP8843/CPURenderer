#ifndef POINTDATA_H
#define POINTDATA_H

#include <DrawingWindow.h>
#include <TexturePoint.h>
#include <glm/glm.hpp>

#include "../helper/Material.h"
#include "../helper/ModelPoint.h"
#include "../objects/Light.h"

namespace FragmentData
{
    struct Data
    {
        glm::vec3 proportion;
        float depth;
        ModelPoint point;

        Data(glm::vec3 proportion, float depth, const ModelPoint& point);

        Data operator+(const Data& other) const;
        Data operator-(const Data& other) const;

        template <typename N>
        Data operator*(const N num)
        {
            return Data(proportion * num, depth * num, point * num);
        }
    };

    struct PrePassData
    {
        float depth;

        explicit PrePassData(float depth);

        PrePassData operator+(const PrePassData& other) const;
        PrePassData operator-(const PrePassData& other) const;

        template <typename N>
        PrePassData operator*(const N num)
        {
            return PrePassData(depth * num);
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

    struct PrePassUniform
    {
        float** depthBuffer;
        size_t width;
        size_t height;

        PrePassUniform(float** depthBuffer, const size_t& width, const size_t& height);
    };
}

#endif //POINTDATA_H
