#pragma once

#include "raytracing/core/Scene.hpp"
#include "raytracing/core/hittableObjects/Box.hpp"
#include "raytracing/core/hittableObjects/Cone.hpp"
#include "raytracing/core/hittableObjects/CsgObject.hpp"
#include "raytracing/core/hittableObjects/Cylinder.hpp"
#include "raytracing/core/hittableObjects/Icosahedron.hpp"
#include "raytracing/core/hittableObjects/Sphere.hpp"
#include "raytracing/core/hittableObjects/SquarePyramid.hpp"
#include "raytracing/core/hittableObjects/TriangularPyramid.hpp"

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
         * Adds a Sphere with the given properties in the scene.
         * @param center the center of the sphere.
         * @param radius the radius
         * @param materialIndex the index of the material in the material list.
         */
        void pushSphere(glm::vec3 center, float radius, unsigned int materialIndex);

        /**
         * Adds an oriented box with the given properties.
         * @param center the center of the box.
         * @param halfSize the half-size on each local axis.
         * @param rotation the Euler rotation in radians around X, Y and Z.
         * @param materialIndex the index of the material in the material list.
         */
        void pushBox(glm::vec3 center, glm::vec3 halfSize, glm::vec3 rotation, unsigned int materialIndex);

        /**
         * Adds an oriented finite cylinder.
         * @param baseCenter the center of the bottom cap before rotation.
         * @param radius the cylinder radius.
         * @param height the cylinder height along local Y.
         * @param rotation the Euler rotation in radians around X, Y and Z.
         * @param materialIndex the index of the material in the material list.
         */
        void pushCylinder(glm::vec3 baseCenter, float radius, float height, glm::vec3 rotation, unsigned int materialIndex);

        /**
         * Adds an oriented finite cone closed by its base cap.
         * @param baseCenter the center of the bottom cap before rotation.
         * @param radius the base radius.
         * @param height the cone height along local Y.
         * @param rotation the Euler rotation in radians around X, Y and Z.
         * @param materialIndex the index of the material in the material list.
         */
        void pushCone(glm::vec3 baseCenter, float radius, float height, glm::vec3 rotation, unsigned int materialIndex);

        void pushSquarePyramid(glm::vec3 baseCenter,
                               glm::vec2 halfBaseSize,
                               float height,
                               glm::vec3 rotation,
                               unsigned int materialIndex);

        void pushTriangularPyramid(glm::vec3 baseCenter,
                                   float baseRadius,
                                   float height,
                                   glm::vec3 rotation,
                                   unsigned int materialIndex);

        void pushIcosahedron(glm::vec3 center, float radius, glm::vec3 rotation, unsigned int materialIndex);

        void pushObject(Scene::ObjectPtr object);

        void pushCsgObject(Scene::ObjectPtr left,
                           Scene::ObjectPtr right,
                           CsgOperation operation,
                           unsigned int materialIndex);

        void pushUnion(Scene::ObjectPtr left, Scene::ObjectPtr right, unsigned int materialIndex);

        void pushSubtraction(Scene::ObjectPtr left, Scene::ObjectPtr right, unsigned int materialIndex);

        void pushIntersection(Scene::ObjectPtr left, Scene::ObjectPtr right, unsigned int materialIndex);
        /**
         * Adds a material with the given properties. The more a sphere is shiny,
         * the more its color is influenced by the next bounce color. The more a sphere is
         * rough, the more the ray is reflect or refract for a dialectric material in a
         * random direction.
         * @param reflectionColor the color reflected by the object
         * @param shininess the level of shiny of the sphere in [0, 1]
         * @param roughness the level of roughness in [0, 1]
         */
        void pushMaterial(const glm::vec3& reflectionColor, float shininess, float roughness);
        /**
         * Adds a material with the given properties. The more a sphere is shiny,
         * the more its color is influenced by the next bounce color. The more a sphere is
         * rough, the more the ray is reflect or refract for a dielectric material in a
         * random direction.
         * @param reflectionColor the color reflected by the object
         * @param emissionColor the emission color
         * @param shininess the shininess level of the sphere in [0, 1]
         * @param roughness the level of roughness in [0, 1]
         * @param emissionPower the emission level of the light in [0, +infty]
         * @param attenuationRadius the attenuation radius of the emission
         * @param refractionIndex the refractionIndex
         */
        void pushMaterial(const glm::vec3& reflectionColor,
                          const glm::vec3& emissionColor,
                          float shininess,
                          float roughness,
                          float emissionPower,
                          float attenuationRadius,
                          float refractionIndex,
                          float translucentAbsorption = 0.45f);

        /**
         * Adds a perfect dielectric material.
         * @param refractionIndex the refraction index
         */
        void pushMaterial(float refractionIndex);

        /**
         * Adds a rough dielectric material.
         * @param refractionIndex the refraction index
         * @param roughness the roughness
         */
        void pushMaterial(float refractionIndex, float roughness);

        /**
         * Adds a rough dielectric material with colored absorption.
         * @param refractionIndex the refraction index
         * @param roughness the roughness
         * @param translucentAbsorption the absorption strength per scene unit
         */
        void pushMaterial(float refractionIndex, float roughness, float translucentAbsorption);

        /**
         * Removes the last sphere added.
         */
        void popSphere();

        /**
         * Gets the scene created.
         * @return the scene.
         */
        const Raytracing::Scene& getScene() const;

        /**
         * Gets the list of spheres for the currentScene.
         * @return a vector of the scene
         */
        std::vector<Raytracing::Sphere> getListOfSphere();
    };
} // namespace Raytracing
