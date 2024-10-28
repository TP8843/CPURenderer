//
// Created by Thomas Parr on 28/10/2024.
//

#ifndef POINTDATA_H
#define POINTDATA_H
#include <Colour.h>
#include <glm/glm.hpp>

namespace PointData
{
    struct FilledData {
        Colour colour;
        glm::vec3 proportion;
        float depth;

        FilledData(const Colour& colour, glm::vec3 proportion, float depth);

        FilledData operator+(const FilledData &other) const;
        FilledData operator-(const FilledData &other) const;

        template<typename N>
        FilledData operator*(const N num)
        {
            return FilledData(colour * num, proportion * num, depth * num);
        }
    };
}

#endif //POINTDATA_H
