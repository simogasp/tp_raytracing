/**
 * This is the App We wants to build with a raytracing.
 */

# include "../controller/Renderer.hpp"


namespace Raytracing
{

    class App
    {
    private:
        /* data */
        uint32_t m_viewportWidth = 0;
        uint32_t m_viewportHeight = 0;

        Renderer renderer;

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
