//
// Created by Thomas Parr on 08/10/2024.
//

#ifndef OBJECT_H
#define OBJECT_H
#include <../../libs/sdw/CanvasPoint.h>
#include <../../libs/sdw/DrawingWindow.h>
#include <../../libs/sdw/ModelTriangle.h>
#include <unordered_map>
#include <vector>

class Model {
public:
    static Model import(const char* objectPath);

    std::vector<ModelTriangle> triangles;

private:
    Model() = default;

    explicit Model(const std::vector<ModelTriangle> &triangles);

    static std::unordered_map<std::string, Material> importMaterials(const std::string &path);
};



#endif //OBJECT_H
