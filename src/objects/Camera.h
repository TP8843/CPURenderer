#ifndef CAMERA_H
#define CAMERA_H

#include <chrono>
#include <../../libs/glm-0.9.7.2/glm/glm.hpp>

class Camera
{
public:
    Camera(glm::vec3 position,
           const glm::mat3& rotation,
           float focalLength,
           float imagePlaneScaling);

    // Get the rotation matrix for any normals
    glm::mat3 getNormalRotationMatrix() const;

    // Move camera by given vector along rotated axis
    void translateRelative(glm::vec3 translation);

    // Move camera by given vector along absolute axis
    void translateAbsolute(glm::vec3 translation);

    // Rotate camera relative to scene using rotation matrix
    void rotate(const glm::mat3& newRotation);

    // Rotate around x-axis by given angle (radians)
    void rotateX(float angle);

    // Rotate around y-axis by given angle (radians)
    void rotateY(float angle);

    // Toggles orbit mode. During orbit mode, no camera controls work
    bool toggleOrbit();

    // Move camera to next stage of orbit. Only works in orbit mode
    void iterateOrbit();

    // Return state of orbit
    bool getOrbit() const;

    // Reset camera position
    void reset();

    // Look at location
    void lookAt(glm::vec3 location);

    glm::vec3 position;
    glm::mat3 rotation;
    float focalLength;
    float imagePlaneScaling;

private:
    // True if camera is orbiting
    bool orbit = false;

    // True if camera moves relative to scene
    bool relative = true;

    glm::vec3 initialPosition;
    glm::mat3 initialRotation;
    std::chrono::milliseconds prevTime{};
};


#endif //CAMERA_H
