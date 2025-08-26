/**
 * This is the App We wants to build with a raytracing.
 */
#pragma once

#include "raytracing/controller/Renderer.hpp"
#include "raytracing/controller/CameraFactory.hpp"
#include "raytracing/controller/SceneFactory.hpp"

#include "glm/glm.hpp"

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
        
        // color helper
        /**Black color. */
        const glm::vec3 black = glm::vec3(0.f, 0.f, 0.f);
        /**White color. */
        const glm::vec3 white = glm::vec3(1.f, 1.f, 1.f);
        /**Red color. */
        const glm::vec3 red = glm::vec3(1.f, 0.f, 1.f);
        /**Blue color. */
        const glm::vec3 blue = glm::vec3(51.f / 255, 77.f / 255, 1.f);
        /**Orange color. */
        const glm::vec3 orange = glm::vec3(0.8f, 0.5f, 0.2f);
        /**Gray color. */
        const glm::vec3 gray = glm::vec3(122.f / 255, 127.f / 255, 128.f / 255);

        // position helper
        /**Red sphere position. */
        const glm::vec3 redPos = glm::vec3(-1.f, 0.f, 0.f);
        /**Floor sphere position. */
        const glm::vec3 floorPos = glm::vec3(0.f, -1001.f, 0.f);
        /**Light / sun sphere position. */
        const glm::vec3 lightPos = glm::vec3(0.f, 10.f, 30.f);
        /**Glass sphere position. */
        const glm::vec3 glassPos = glm::vec3(1.f, 0.f, 0.f);

        // shinyness helper
        /**A full mat shinyness. */
        const float mat = 0.0f;
        /**A middle shinyness. */
        const float midShiny = 0.5f;
        /**A full shiny material. */
        const float shiny = 1.f;

        // roughness helper
        /**A full roughness material. */
        const float fullRoughness = 1.0f;
        /**A middle roughness material */
        const float midRoughness = .5f;
        /**A material without roughness. */
        const float noRoughness = 0.0f;

        // emissionPower helper
        /**A non-emissive material. */
        const float noEmissionPower = 0.F;
        /**A normal emissive material. */
        const float normalEmissionPower = 1.F;
        /**A brightness emissive material. */
        const float midEmissionPower = 2.f;
        /**A very bright emissive material. */
        const float fullEmissionPower = 10.f;

        // refraction index helper
        /**A opaque material. */
        const float noTranslucid = 0.f;
        /**A air material. */
        const float airTranslucid = 1.f;
        /**A water material. */
        const float waterTranslucid = 1.33f;
        /**A plexiglass material material. */
        const float plexiGlassTranslucid = 1.5f;

        
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
        Camera camera;
        /**
         * The scene rendered in the viewport.
         */
        SceneFactory scene;
        /**
         * The renderer to do the render in the viewport.
         */
        Renderer renderer;

        /** The field of View of the Camera. */
        float focal = 2.f;

    public:
        /**
         * Creates an App.
         */
        App();

        /**
         * Destroy an App.
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
