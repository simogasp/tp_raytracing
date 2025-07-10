#include "App.hpp"

namespace Raytracing
{
    App::App()
        : camera(), scene()
    {
        camera.setCameraPosition(glm::vec3(0, 0, 2));
        camera.setLookAt(glm::vec3(0));
        camera.setDegreeHorizontalFOV(180);
        camera.setUpVector({0, 1, 0});
        camera.setNear(0.1);
        camera.setFar(100);
    }

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
        if (ImGui::SliderInt("Camera Horizontal Fov", &fovDegree, 0, 180))
        {
            camera.setDegreeHorizontalFOV(fovDegree);
        }
        // for (size_t i = 0; i < scene.getListOfSphere(); i++)
        // {
            
        // }
        

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
        // resize
        camera.onResize(m_viewportWidth, m_viewportHeight);
        renderer.onResize(m_viewportWidth, m_viewportHeight);

        // render
        renderer.Render(scene.getScene(), camera.getCamera());
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
            camera.forward();
            updateRays = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_S))
        {
            camera.backward();
            updateRays = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Q))
        {
            camera.left();
            updateRays = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_D))
        {
            camera.right();
            updateRays = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Space))
        {
            camera.up();
            updateRays = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_LeftCtrl))
        {
            camera.down();
            updateRays = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_A))
        {
            camera.rotateAntiClockWise();
            updateRays = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_E))
        {
            camera.rotateClockWise();
            updateRays = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_LeftArrow))
        {
            camera.lookLeft();
            updateRays = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_RightArrow))
        {
            camera.lookRight();
            updateRays = true;
        }

        if (updateRays)
            camera.updateRay();
    }
} // namespace Raytracing