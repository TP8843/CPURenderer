#include "Scene.h"


std::vector<ModelTriangle> Scene::getTransformedTriangles() const
{
    auto triangles = std::vector<ModelTriangle>();

    for (const Model* model : models)
    {
        auto transformedTriangles = model->getTransformedTriangles();
        triangles.insert(triangles.end(), transformedTriangles.begin(), transformedTriangles.end());
    }

    return triangles;
}

std::vector<ModelTriangle> Scene::getRasterPreparedTriangles(const Transformation& camera) const
{
    auto triangles = std::vector<ModelTriangle>();

    for (const Model* model : models)
    {
        auto transformedTriangles = model->getRasterPreparedTriangles(camera);
        triangles.insert(triangles.end(), transformedTriangles.begin(), transformedTriangles.end());
    }

    return triangles;
}

Scene::Scene(std::vector<Model*>& models, MaterialMap& materials, Transformation& camera, Transformation& light) :
    models(models), materials(materials), camera(camera), light(light)
{
}
