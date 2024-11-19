#ifndef SCENE_H
#define SCENE_H
#include "Model.h"


class Scene {
public:
    std::vector<Model*>& models;
    MaterialMap& materials;
    Transformation& camera;
    Transformation& light;

    // Gets all transformed triangles for scene (Expensive)
    std::vector<ModelTriangle> getTransformedTriangles() const;

    // Get clipped and transformed triangles for raster render (Expensive)
    std::vector<ModelTriangle> getRasterPreparedTriangles(const Transformation& camera) const;

    Scene(std::vector<Model*>& models, MaterialMap& materials, Transformation& camera, Transformation& light);
private:
};



#endif //SCENE_H
