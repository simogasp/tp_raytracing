# pragma once

# include "hittableObject/Sphere.hpp"

namespace Raytracing
{
    struct HitPayload
    {
        float hitDistance;
        glm::vec3 worldPosition;
        glm::vec3 worldNormal;
        uint32_t objectIndex;
    };
    
} // namespace Raytracing
