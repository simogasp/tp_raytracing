#pragma once

#include "stdint.h"
#include "raytracing/utils/vector.hpp"
#include "raytracing/core/Ray.hpp"
#include "raytracing/core/Camera.hpp"

#define EPSILON_SPHERE 1e-4

namespace Raytracing
{
    struct Sphere
    {
        glm::vec3 center;
        float radius;
        uint32_t materialIndex;

        double intersect(const Camera *camera, const Ray *ray) const;
    };

} // namespace Raytracing
