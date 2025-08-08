#pragma once

#include "stdint.h"
#include "raytracing/core/Ray.hpp"

#define EPSILON_SPHERE 1e-4

namespace Raytracing
{
    struct Material
    {
        /**
         * The air refraction index.
         */
        static constexpr float AIR_REFRACTION_INDEX = 1.0f;

        /**
         * The reflection color of the material.
         */
        glm::vec3 reflection;
        /**
         * The emision color of the material.
         */
        glm::vec3 emission = glm::vec3(0.f);
        /**
         * The shinyness of the material.
         */
        float shinyness;
        /**
         * The roughness of the material.
         */
        float roughness;
        /**
         * The emission power of the material.
         */
        float emissionPower = 0;
        /**
         * The attenuation radius of the material.
         */
        float attenuationRadius = 0;
        /**
         * The refraction index of the material.
         */
        float refractionIndex = 0;

        /**
         * Gets the emissionPower times the emissions color.
         * @return a vector of color
         */
        glm::vec3 getEmission() const {
            return emissionPower * emission;
        }
    };

} // namespace Raytracing
