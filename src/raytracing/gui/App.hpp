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
        const glm::vec3 camPos = glm::vec3(-56.443f, -11.0f, 40.181f);
        const glm::vec3 lookAtPos = glm::vec3(-55.641f, -11.0f, 39.584f);
        const glm::vec3 camPos2 = glm::vec3(-6.709, 0, 3.160);
        const glm::vec3 lookAtPos2 = glm::vec3(-5.907, 0, 2.563);
        const glm::vec3 camPos3 = glm::vec3(-56.443f, 11.0f, 40.181f);
        const glm::vec3 lookAtPos3 = glm::vec3(-55.641f, 11.0f, 39.584f);
        const glm::vec3 camPos4 = glm::vec3(-0.443f, .0f, 5.715f);
        const glm::vec3 lookAtPos4 = glm::vec3(-0.335f, 0.0f, 4.724f);
        /* data */
        uint32_t m_viewportWidth = 0;
        uint32_t m_viewportHeight = 0;
        CameraFactory camera;
        SceneFactory scene;
        Renderer renderer;

        int fovDegree = 45;
        int camSetting = 1;
    public:
        App();

        ~App() = default;

        void OnUIRender();
        
    private:
        void Render();
        void keyboardHandler();
        void handleCamPreset(const int node_clicked);
    };
}
