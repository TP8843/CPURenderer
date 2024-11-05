#pragma once

#include <glm/glm.hpp>
#include <array>
#include <TextureMap.h>

#include "TexturePoint.h"
#include "../../src/helper/ModelPoint.h"

struct ModelTriangle
{
    std::array<ModelPoint, 3> vertices{};
    std::string material{};
    glm::vec3 normal{};

    ModelTriangle();
    ModelTriangle(ModelPoint v0,
                  ModelPoint v1,
                  ModelPoint v2,
                  const glm::vec3& normal,
                  std::string material);

    friend std::ostream& operator<<(std::ostream& os, const ModelTriangle& triangle);
};
