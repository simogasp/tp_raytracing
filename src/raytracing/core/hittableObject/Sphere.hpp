# pragma once

# include "stdint.h"
# include "raytracing/utils/vector.hpp"
# include "raytracing/core/Ray.hpp"

# define EPSILON_SPHERE 1e-4

namespace Raytracing
{
    struct Sphere
    {
        glm::vec3 center;
        float radius;
        glm::vec3 reflection;
        glm::vec3 emission;
    };

} // namespace Raytracing
