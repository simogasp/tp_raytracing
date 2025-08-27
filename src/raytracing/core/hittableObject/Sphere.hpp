#pragma once

#include "raytracing/core/Ray.hpp"
#include "raytracing/core/Camera.hpp"

#define EPSILON_SPHERE 1e-4

namespace Raytracing
{
    struct Sphere
    {
        /**
         * The center of the sphere.
         */
        glm::vec3 center;
        /**
         * The radius of the radius.
         */
        float radius;
        /**
         * The material Index.
         */
        uint32_t materialIndex;

        /**
         * Returns the distance of intersections (-1 if don't intersect).
         * @return the distance
         */
        double intersect(const Camera *camera, const Ray *ray) const;

        /**
         * Returns the normal for a position around the the sphere.
         * @return the normal
         */
        glm::vec3 getNormal(const glm::vec3 position) const;
    };

} // namespace Raytracing
