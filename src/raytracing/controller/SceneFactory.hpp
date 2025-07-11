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
        void popSphere();
        Raytracing::Scene getScene();
        static Raytracing::Scene getDefaultScene();
        std::vector<Raytracing::Sphere> getListOfSphere();
    };
    
} // namespace Raytracing
