# pragma once

# include "raytracing/core/Scene.hpp"

namespace Raytracing
{
    class SceneFactory
    {
    private:
        Scene currentScene;
    public:
        SceneFactory();
        ~SceneFactory() = default;
        void createNewScene();
        void addRandomSphereToScene();
        void pushSphere(const glm::vec3 center, const float radius, const uint materialIndex);
        void pushMaterial(const glm::vec3 reflectionColor, const glm::vec3 emissionColor, const float shinyness, const float roughness, const float emissionPower);
        void popSphere();
        Raytracing::Scene getScene();
        static Raytracing::Scene getDefaultScene();
        std::vector<Raytracing::Sphere> getListOfSphere();
    };
} // namespace Raytracing
