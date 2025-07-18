# pragma once

# include "stdint.h"
# include "raytracing/utils/vector.hpp"
# include "raytracing/core/Ray.hpp"

# define EPSILON_SPHERE 1e-4

namespace Raytracing
{
    struct Material
    {
        glm::vec3 reflection;
        glm::vec3 emission = glm::vec3(0.f);
        float shinyness;
        float roughness;
        float emissionPower = 0;
        float attenuationRadius = 100;
        float refractiveIndex = 0;

        glm::vec3 getEmission() const {
            return emissionPower * emission;
        }
    };

} // namespace Raytracing
