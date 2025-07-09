# pragma once

# include "stdint.h"
# include "raytracing/utils/vector.hpp"
# include "raytracing/core/Ray.hpp"

# define EPSILON_SPHERE 1e-4

namespace Raytracing
{
    class Sphere
    {
    private:
        float radius;
        Vector3 center;
        // Vector3 emission;
        Vector3 reflection;

    public:
        Sphere(const float theRadius, const Vector3& pos, const Vector3& reflection);
        ~Sphere() = default;
        
        [[nodiscard]]
        bool intersect(Ray *ray);
    };

		

} // namespace Raytracing
