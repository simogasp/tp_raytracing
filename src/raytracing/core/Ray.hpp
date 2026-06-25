# pragma once

#include "glm/glm.hpp"

namespace Raytracing
{
    struct Ray
    {
        /**
         * The origin of the ray.
         */
        glm::vec3 origin;
        /**
         * The direction of the ray.
         */
        glm::vec3 direction;
        /**
         * The number of bounces that the ray have done.
         */
        unsigned int bounce = 0;
        /**
         * The material currently containing the ray, or -1 if the ray is in air.
         */
        int mediumMaterialIndex = -1;
    };
} // namespace Raytracing
