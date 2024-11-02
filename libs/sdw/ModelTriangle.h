#pragma once

#include <glm/glm.hpp>
#include <array>
#include <TextureMap.h>
#include <../../src/helper/Material.h>

#include "TexturePoint.h"

struct ModelTriangle
{
    std::array<glm::vec3, 3> vertices{};
    std::array<TexturePoint, 3> texturePoints{};
    std::string material{};
    glm::vec3 normal{};

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

    friend std::ostream& operator<<(std::ostream& os, const ModelTriangle& triangle);
};
