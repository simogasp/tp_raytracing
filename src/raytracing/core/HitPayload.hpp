#pragma once

#include "hittableObject/Sphere.hpp"

namespace Raytracing
{
    struct HitPayload
    {
        /** The hit distance. */
        float hitDistance;
        /** The bounce position. */
        glm::vec3 worldPosition;
        /** The bounce normal */
        glm::vec3 worldNormal;
        /** The objectIndex */
        uint32_t objectIndex;
        /** Whether the bounce is inside the box or not. */
        bool inside;
    };
    
} // namespace Raytracing
