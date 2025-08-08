# pragma once

#include "glm/glm.hpp"

namespace Raytracing
{
    struct Ray
    {
        /**
         * The orgin of the ray.
         */
        glm::vec3 origin;
        /**
         * The direction of the ray.
         */
        glm::vec3 direction;
        /**
         * The number of bounces that the ray have done.
         */
        uint bounce = 0;
    };
} // namespace Raytracing
