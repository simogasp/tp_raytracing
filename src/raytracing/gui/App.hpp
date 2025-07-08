/**
 * This is the App We wants to build with a raytracing.
 */

# include "raytracing/controller/Renderer.hpp"


namespace Raytracing
{

    class App
    {
    private:
        /* data */
        uint32_t m_viewportWidth = 0;
        uint32_t m_viewportHeight = 0;

        Renderer renderer;
        int fovDegree = 179;
        float fov = toRadian(fovDegree);

    public:
        App();

        ~App();
        
        void OnUIRender()
        {
            ImGuiIO &io = ImGui::GetIO();

            // Dockspace Window
            ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

            
            // Help panel
            ImGui::Begin("Help");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();

            // Settings panel
            ImGui::Begin("Settings");
            if (ImGui::SliderInt("Fov", &fovDegree, 0, 180))
            {
                fov = toRadian(fovDegree);
                renderer.setFov(fov);
            }
            ImGui::End();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            // Dockspace Viewport
            ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoScrollbar);

            m_viewportWidth = ImGui::GetContentRegionAvail().x;
            m_viewportHeight = ImGui::GetContentRegionAvail().y;

            // call the render
            Render();

            // if there is a image : draw it
            const GLuint imageTextureId = renderer.getTextureId();
            if (imageTextureId)
            {
                ImGui::Image((ImTextureID) imageTextureId,
                ImVec2(renderer.getWidth(), renderer.getHeight()));
            }
            ImGui::End();
            ImGui::PopStyleVar();

            if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Z))
            {
                renderer.cameraForward();
            }
            if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_S))
            {
                renderer.cameraBackward();
            }
            if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Q))
            {
                renderer.cameraLeftShift();
            }
            if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_D))
            {
                renderer.cameraRightShift();
            }
            if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Space))
            {
                renderer.cameraUpShift();
            }
            if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_LeftCtrl))
            {
                renderer.cameraDownShift();
            }

        }

        void Render()
        {

            renderer.OnResize(m_viewportWidth, m_viewportHeight);
            renderer.Render();

        }
    };

    App::App()
    {
    }

    App::~App()
    {
    }
}
