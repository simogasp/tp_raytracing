#include "App.hpp"

namespace Raytracing
{

    void App::OnUIRender()
    {
        ImGuiIO &io = ImGui::GetIO();



        // Dockspace Window
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        // Help panel
        ImGui::Begin("Help");
        if (ImGui::TreeNode("Move in the scene"))
        {
            // int id = 0;
            // ImGui::PushID(id++);
            ImGui::Text("Z to move forward");
            ImGui::Text("S to move backward");
            ImGui::Text("D to move right");
            ImGui::Text("Q to move right");
            ImGui::Text("A to rotate camera Anti clockwise");
            ImGui::Text("E to rotate camera clockwise");
            ImGui::Text("Space to move Up");
            ImGui::Text("Ctrl to move Down");
            ImGui::Text("ArrowLeft to look left side Down");
            ImGui::Text("ArrowRight to look right side Down");
            // ImGui::PopID();
            ImGui::TreePop();
        }
        ImGui::Text("Keys down:");
        struct funcs
        {
            static bool IsLegacyNativeDupe(ImGuiKey) { return false; }
        };
        for (ImGuiKey key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END; key = (ImGuiKey)(key + 1))
        {
            if (funcs::IsLegacyNativeDupe(key) || !ImGui::IsKeyDown(key))
                continue;
            ImGui::SameLine();
            ImGui::Text((key < ImGuiKey_NamedKey_BEGIN) ? "\"%s\"" : "\"%s\" %d", ImGui::GetKeyName(key), key);
        }
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
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

        m_viewportWidth = ImGui::GetContentRegionAvail().x;
        m_viewportHeight = ImGui::GetContentRegionAvail().y;

        // call the render
        Render();

        // call the keyboard handler
        keyboardHandler();
        io.ClearInputKeys();
        io.ClearInputCharacters();

        // if there is a image : draw it
        const GLuint imageTextureId = renderer.getTextureId();
        if (imageTextureId)
        {
            ImGui::Image((ImTextureID)imageTextureId,
                ImVec2(renderer.getWidth(), renderer.getHeight()));
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void App::Render()
    {

        renderer.OnResize(m_viewportWidth, m_viewportHeight);
        renderer.Render();
    }

    void App::keyboardHandler()
    {
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Escape))
        {
            exit(0);
        }

        bool updateRays = false;
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Z))
        {
            renderer.cameraForward();
            updateRays = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_S))
        {
            renderer.cameraBackward();
            updateRays = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Q))
        {
            renderer.cameraLeftShift();
            updateRays = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_D))
        {
            renderer.cameraRightShift();
            updateRays = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Space))
        {
            renderer.cameraUpShift();
            updateRays = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_LeftCtrl))
        {
            renderer.cameraDownShift();
            updateRays = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_A))
        {
            renderer.cameraRotateAntiClockWise();
            updateRays = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_E))
        {
            renderer.cameraRotateClockWise();
            updateRays = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_LeftArrow))
        {
            renderer.cameraLookLeft();
            updateRays = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_RightArrow))
        {
            renderer.cameraLookRight();
            updateRays = true;
        }

        if (updateRays)
            renderer.updateRay();
    }
} // namespace Raytracing