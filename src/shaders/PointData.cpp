//
// Created by Thomas Parr on 28/10/2024.
//

#include "PointData.h"

PointData::FilledData::FilledData(const Colour& colour, const glm::vec3 proportion, const float depth) :
        colour(colour), proportion(proportion), depth(depth)
{}

PointData::FilledData PointData::FilledData::operator+(const FilledData& other) const
{
        return {
                colour + other.colour,
                proportion + other.proportion,
                depth + other.depth
        };
}

PointData::FilledData PointData::FilledData::operator-(const FilledData& other) const
{
        return {
                colour - other.colour,
                proportion - other.proportion,
                depth - other.depth
        };
}