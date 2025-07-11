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
        void pushSphere(const glm::vec3 center, const float radius, const glm::vec3 reflection, const glm::vec3 emission);
        void popSphere();
        Raytracing::Scene getScene();
        static Raytracing::Scene getDefaultScene();
        std::vector<Raytracing::Sphere> getListOfSphere();
    };
    
} // namespace Raytracing
