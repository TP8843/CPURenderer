//
// Created by Thomas Parr on 08/10/2024.
//

#ifndef OBJECT_H
#define OBJECT_H
#include <ModelTriangle.h>


class Model {
public:
    static Model import(const char* objectPath);
    std::vector<ModelTriangle> triangles;

private:
    Model() = default;
    explicit Model(const std::vector<ModelTriangle>& triangles);

    static std::unordered_map<std::string, Colour> importMaterials(const std::string &path);
};



#endif //OBJECT_H
