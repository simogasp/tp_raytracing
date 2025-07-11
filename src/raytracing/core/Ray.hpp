# pragma once

# include "imgui.h"
# include "glm/glm.hpp"

namespace Raytracing
{
    struct Ray
    {
        glm::vec3 origin;
        glm::vec3 direction;
    };    
} // namespace Raytracing
