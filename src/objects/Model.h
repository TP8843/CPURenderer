#ifndef OBJECT_H
#define OBJECT_H
#include <../../libs/sdw/ModelTriangle.h>
#include <vector>

#include "../helper/MaterialMap.h"

class Model {
public:
    static Model import(const char* objectPath);

    MaterialMap materials;
    std::vector<ModelTriangle> triangles;

private:
    Model() = default;

    explicit Model(const std::vector<ModelTriangle> &triangles, const MaterialMap& materials);

    static MaterialMap importMaterials(const std::string &path);
};



#endif //OBJECT_H
