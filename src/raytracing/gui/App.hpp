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
        /** Cam 1 position */
        const glm::vec3 camPos = glm::vec3(-56.443f, -11.0f, 40.181f);
        /** LookAt 1 position */
        const glm::vec3 lookAtPos = glm::vec3(-55.641f, -11.0f, 39.584f);
        
        /** Cam 2 position */
        const glm::vec3 camPos2 = glm::vec3(-6.709, 0, 3.160);
        /** LookAt 2 position */
        const glm::vec3 lookAtPos2 = glm::vec3(-5.907, 0, 2.563);
        
        /** Cam 3 position */
        const glm::vec3 camPos3 = glm::vec3(-56.443f, 11.0f, 40.181f);
        /** LookAt 3 position */
        const glm::vec3 lookAtPos3 = glm::vec3(-55.641f, 11.0f, 39.584f);
        
        /** Cam 4 position */
        const glm::vec3 camPos4 = glm::vec3(-0.443f, .0f, 5.715f);
        /** LookAt 4 position */
        const glm::vec3 lookAtPos4 = glm::vec3(-0.335f, 0.0f, 4.724f);
        
        /** Cam 5 position */
        const glm::vec3 camPos5 = glm::vec3(0.f, .0f, 2.f);
        /** LookAt 5 position */
        const glm::vec3 lookAtPos5 = glm::vec3(0.f);
        
        /* data */
        /** 
         * The viewPort width.
         */ 
        uint32_t m_viewportWidth = 0;
        /**
         * The viewPort height.
         */
        uint32_t m_viewportHeight = 0;

        /**
         * The Camera used in the viewPort.
         */
        CameraFactory camera;
        /**
         * The scene rendered in the viewport.
         */
        SceneFactory scene;
        /**
         * The renderer to do the render in the viewport.
         */
        Renderer renderer;

        /** The field of View of the Camera. */
        int fovDegree = 45;

    public:
        /**
         * Creates a App.
         */
        App();

        /**
         * Destroy a App.
         */
        ~App() = default;

        /**
         * Called By ImGUI to render at each frame;
         */
        void OnUIRender();
        
    private:
        /**
         * Render procedure to create a render on the viewPort.
         */
        void Render();
        /**
         * Handles the keyboard input.
         */
        void keyboardHandler();
        
        /**
         * Changes the camera to the given preset id.
         * @param node_clicked the id of the preset
         */
        void handleCamPreset(const int node_clicked);
    };
}
