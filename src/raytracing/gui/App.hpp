/**
 * This is the App We wants to build with a raytracing.
 */
# pragma once

# include "raytracing/controller/Renderer.hpp"
# include "raytracing/controller/CameraFactory.hpp"
# include "raytracing/controller/SceneFactory.hpp"

# include "glm/glm.hpp"

namespace Raytracing
{

    class App
    {
    private:
        /* data */
        uint32_t m_viewportWidth = 0;
        uint32_t m_viewportHeight = 0;
        CameraFactory camera;
        SceneFactory scene;
        Renderer renderer;

        int fovDegree = 45;

    public:
        App();

        ~App() = default;

        void OnUIRender();
        void Render();
        

        void keyboardHandler();
        
    };
}
