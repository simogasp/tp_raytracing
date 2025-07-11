# pragma once

# include "glm/glm.hpp"

namespace Raytracing
{
    struct Light
    {
        double intensity;
        glm::vec3 direction;
    };
    
} // namespace Raytracing
