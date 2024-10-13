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

    static CanvasPoint projectVertexOntoCanvasPoint(glm::vec3 cameraPosition, float focalLength, float imagePlaneScaling, glm::vec3 vertexPosition, glm::vec2 canvasSize);

    void pointcloudRender(DrawingWindow &window, glm::vec3 cameraPosition, float focalLength, float imagePlaneScaling);
    void wireframeRender(DrawingWindow &window, glm::vec3 cameraPosition, float focalLength, float imagePlaneScaling);
    void rasterRender(DrawingWindow &window, glm::vec3 cameraPosition, float focalLength, float imagePlaneScaling);

    std::vector<ModelTriangle> triangles;

private:
    Model() = default;

    explicit Model(const std::vector<ModelTriangle> &triangles);

    static std::unordered_map<std::string, Colour> importMaterials(const std::string &path);
};



#endif //OBJECT_H
