# include "App.hpp"
# include "glm/gtc/type_ptr.hpp"
namespace Raytracing
{
    App::App()
        : camera(), scene()
    {
        // scene.addRandomSphereToScene();
        const glm::vec3 black(0.f, 0.f, 0.f);
        const glm::vec3 red(1.f, 0.f, 1.f);
        const glm::vec3 blue(51.f/255, 77.f/255, 1.f);
        const glm::vec3 orange(0.8f, 0.5f, 0.2f);

        const glm::vec3 redPos(-1.f, 0.f, 0.f);
        const glm::vec3 floorPos(0.f, 1001.f, 0.f);
        const glm::vec3 lightPos(1.f, -10.f, -30.f);

        const float mat = 0.5f;
        
        const float fullRoughness = 1.0f;
        const float midRoughness = .5f;
        const float noRoughness = 0.0f;


        // materials
        scene.pushMaterial(red, black, mat, fullRoughness, 0.f);
        scene.pushMaterial(blue, black, 1, fullRoughness, 0.f);
        scene.pushMaterial(orange, orange, mat, fullRoughness, 15.f);

        // spheres
        scene.pushSphere(redPos, 1.f, 0);
        scene.pushSphere(floorPos, 1000.f, 1);
        scene.pushSphere(lightPos, 20.f, 2);

        // camera
        camera.setCameraPosition(glm::vec3(-6.709, 0, 3.160));
        camera.setLookAt(glm::vec3(-5.907, 0, 2.563));
        camera.setDegreeHorizontalFOV(45);
        camera.setUpVector({0, 1, 0});
        camera.setNear(0.1);
        camera.setFar(10000);
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
        if (ImGui::SliderInt("Camera Horizontal Fov", &fovDegree, 0, 50))
        {
            camera.setDegreeHorizontalFOV(fovDegree);
            renderer.resetAcc();
        }
        glm::vec3 pos = camera.getCamera().getPosition();
        if (ImGui::DragFloat3("Camera position", glm::value_ptr(pos)))
        {
            camera.setCameraPosition(pos);
            renderer.resetAcc();
        }
        glm::vec3 lookAt = camera.getCamera().getLookAt();
        if (ImGui::DragFloat3("Camera lookAt", glm::value_ptr(lookAt)))
        {
            camera.setLookAt(lookAt);
            renderer.resetAcc();
        }

        if (ImGui::Button("Reset"))
        {
            renderer.resetAcc();
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
        {
            camera.updateRay();
            renderer.resetAcc();
        }

    }
} // namespace Raytracing