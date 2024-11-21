#pragma once

#include <glm/glm.hpp>
#include <array>

#include "TextureMap.h"
#include "TexturePoint.h"

struct ModelTriangle
{
    std::array<glm::vec3, 3> vertices{};
    std::array<TexturePoint, 3> texturePoints{};
    std::array<glm::vec3, 3> vertexNormals{};
    std::string material{};
    glm::vec3 normal{};
    bool customNormals{};

    ModelTriangle();
    ModelTriangle(const glm::vec3& v0,
                  const glm::vec3& v1,
                  const glm::vec3& v2,
                  const glm::vec3& normal,
                  std::string material);

    ModelTriangle(const glm::vec3& v0, const TexturePoint& t0,
                  const glm::vec3& v1, const TexturePoint& t1,
                  const glm::vec3& v2, const TexturePoint& t2,
                  const glm::vec3& normal,
                  std::string material);

    ModelTriangle(const glm::vec3& v0, const glm::vec3& n0,
          const glm::vec3& v1, const glm::vec3& n1,
          const glm::vec3& v2, const glm::vec3& n2,
          const glm::vec3& normal,
          std::string material);

    ModelTriangle(const glm::vec3& v0, const TexturePoint& t0, const glm::vec3& n0,
              const glm::vec3& v1, const TexturePoint& t1, const glm::vec3& n1,
              const glm::vec3& v2, const TexturePoint& t2, const glm::vec3& n2,
              const glm::vec3& normal,
              std::string material);

    friend std::ostream& operator<<(std::ostream& os, const ModelTriangle& triangle);
};
