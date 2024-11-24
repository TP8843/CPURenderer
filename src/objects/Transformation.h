#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H
#include <glm/glm.hpp>


struct Transformation {
    glm::vec3 position;
    glm::mat3 rotation;
    float scale = 1.0f;

    Transformation() = default;
    Transformation(const glm::vec3& position, const glm::mat3& rotation, const float& scale);

    // Transform a vector by the current transformation
    glm::vec3 transformPoint(glm::vec3 point) const;

    // Get rotation for normal/orthogonal vectors
    glm::mat3 getNormalRotationMatrix() const;

    // Reset position to initial state
    void reset();

    // Look at location
    void lookAt(glm::vec3 location);

    // Translate relative to world
    void translateAbsolute(const glm::vec3& translation);

    // Translate relative to current rotation
    void translateRelative(const glm::vec3& translation);

    // Rotate camera relative to scene using rotation matrix
    void rotate(const glm::mat3& newRotation);

    // Rotate around x-axis by given angle (radians)
    void rotateX(float angle);

    // Rotate around y-axis by given angle (radians)
    void rotateY(float angle);

    Transformation& operator=(const Transformation& other);
    Transformation operator+(const Transformation &other) const;
    Transformation operator-(const Transformation &other) const;

    template<typename N>
    Transformation operator*(const N num) const
    {
        return {
            position * num,
            rotation * num,
            scale * num
        };
    }

private:
    glm::vec3 initialPosition;
    glm::mat3 initialRotation;
    float initialScale = 1.0f;
};



#endif //TRANSFORMATION_H
