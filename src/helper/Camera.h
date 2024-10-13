#ifndef CAMERA_H
#define CAMERA_H

#include <../../libs/glm-0.9.7.2/glm/glm.hpp>

class Camera
{
public:
    Camera(glm::vec3 position,
           const glm::mat3& rotation,
           float focalLength,
           float imagePlaneScaling);

    // Rotate camera using rotation matrix
    void rotate(const glm::mat3& newRotation);

    // Rotate around x-axis by given angle (radians)
    void rotateX(float angle);

    // Rotate around y-axis by given angle (radians)
    void rotateY(float angle);

    // Move camera by given vector along rotated axis
    void translateRelative(glm::vec3 translation);

    // Move camera by given vector along absolute axis
    void translateAbsolute(glm::vec3 translation);

    glm::vec3 position;
    glm::mat3 rotation;
    float focalLength;
    float imagePlaneScaling;

private:

};


#endif //CAMERA_H
