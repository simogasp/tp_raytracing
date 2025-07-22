#pragma once

#include "raytracing/core/Scene.hpp"

namespace Raytracing
{
    class SceneFactory
    {
    private:
        /**
         * The Scene created.
         */
        Scene currentScene;

    public:
        /**
         * Gets a simple scene.
         * @return a scene
         */
        static Raytracing::Scene getDefaultScene();
        
        /**
         * Creates a SceneFactory.
         */
        SceneFactory();
        /**
         * Destroys a SceneFactory.
         */
        ~SceneFactory() = default;
        /**
         * Creates a new scene.
         */
        void createNewScene();
        /**
         * Adds a random sphere in a range of 10 around origin and with a random radius below 3 and a random material.
         */
        void addRandomSphereToScene();

        /**
         * Adds a Sphere with the given properties in the scene.
         * @param center the center of the sphere.
         * @param radius the radius
         * @param materialIndex the index of the material in the material list.
         */
        void pushSphere(const glm::vec3 center, const float radius, const uint materialIndex);
        /**
         * Adds a material with the given properties. The more a sphere is shiny,
         * the more its color is influenced by the next bounce color. The more a sphere is
         * rough, the more the ray is reflect or refract for a dialectric material in a
         * random direction.
         * @param reflectionColor the color reflected by the object
         * @param shinyness the level of shiny of the sphere in [0, 1]
         * @param roughness the level of roughness in [0, 1]
         */
        void pushMaterial(const glm::vec3 reflectionColor, const float shinyness, const float roughness);
        /**
         * Adds a material with the given properties. The more a sphere is shiny,
         * the more its color is influenced by the next bounce color. The more a sphere is
         * rough, the more the ray is reflect or refract for a dialectric material in a
         * random direction.
         * @param reflectionColor the color reflected by the object
         * @param emissionColor the emission color
         * @param shinyness the level of shiny of the sphere in [0, 1]
         * @param roughness the level of roughness in [0, 1]
         * @param emissionPower the level of emission of the light in [0, +infty]
         * @param attenuationRadius the radius of attenuation of the emission
         * @param refractionIndex the refractionIndex
         */
        void pushMaterial(const glm::vec3 reflectionColor,
                          const glm::vec3 emissionColor,
                          const float shinyness,
                          const float roughness,
                          const float emissionPower,
                          const float attenuationRadius,
                          const float refractionIndex);

        /**
         * Adds a perfect dielectric material.
         * @param refractionIndex the refraction index
         */
        void pushMaterial(const float refractionIndex);

        /**
         * Adds a rough dielectric material.
         * @param refractionIndex the refraction index
         * @param roughness the roughness
         */
        void pushMaterial(const float refractionIndex, const float roughness);

        /**
         * Removes the last sphere added.
         */
        void popSphere();

        /**
         * Gets the scene created.
         * @return the scene.
         */
        Raytracing::Scene getScene();

        /**
         * Gets the list of spheres of the currentScene.
         * @return a vector of scene
         */
        std::vector<Raytracing::Sphere> getListOfSphere();
    };
} // namespace Raytracing
