#pragma once

#include "HittableObject.hpp"
#include "raytracing/core/Ray.hpp"
#include "raytracing/core/Camera.hpp"

#define EPSILON_SPHERE 1e-4

namespace Raytracing
{
    struct Sphere : public HittableObject
    {
        Sphere(const glm::vec3& newCenter, float newRadius, uint32_t newMaterialIndex)
            : HittableObject(newMaterialIndex), center(newCenter), radius(newRadius)
        {
        }

        // The center of the sphere.
        glm::vec3 center;

        // The radius of the sphere.
        float radius;

        double sdf(const glm::vec3& position) const override;

        [[nodiscard]] glm::vec3 getNormal(glm::vec3 position) const override;
    };

} // namespace Raytracing
