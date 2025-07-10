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
        glm::vec3 center;
        // glm::vec3 emission;
        glm::vec3 reflection;

    public:
        Sphere(const float theRadius, const glm::vec3& pos, const glm::vec3& reflection);
        ~Sphere() = default;
        
        [[nodiscard]]
        bool intersect(Ray *ray);
    };

		

} // namespace Raytracing
