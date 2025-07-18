# pragma once

# include "hittableObject/Sphere.hpp"

namespace Raytracing
{
    struct HitPayload
    {
        // the hit distance
        float hitDistance;
        // the bounce position
        glm::vec3 worldPosition;
        // the bounce normal
        glm::vec3 worldNormal;
        // te objectIndex
        uint32_t objectIndex;
        // wether the bounce is inside the box or not
        bool inside;
    };
    
} // namespace Raytracing
