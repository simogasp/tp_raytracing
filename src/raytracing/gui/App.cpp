#include "App.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
namespace Raytracing
{
    App::App()
        : camera(), scene()
    {
        // materials
        scene.pushMaterial(red, mat, fullRoughness);
        scene.pushMaterial(gray, shiny, noRoughness);
        scene.pushMaterial(orange, orange, mat, fullRoughness, normalEmissionPower, 1000.f, noTranslucid);
        scene.pushMaterial(white,
                white,
                shiny,
                0.0005f,
                0.f,
                0.f,
                plexiGlassTranslucid);

        // spheres
        scene.pushSphere(redPos, 1.f, 0);
        // scene.pushSphere(floorPos, 1000.f, 1);
        // scene.pushSphere(lightPos, 20.f, 2);
        // scene.pushSphere(glassPos, 1.f, 3);

        // camera
        camera.setPosition(camPos5);
        camera.setLookAt(lookAtPos5);
        camera.setFocal(0.02);
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
            ImGui::Text("Z to move forward");
            ImGui::Text("S to move backward");
            ImGui::Text("D to move right");
            ImGui::Text("Q to move right");
            ImGui::Text("A to rotate camera clockwise");
            ImGui::Text("E to rotate camera Anti clockwise");
            ImGui::Text("Space to move Up");
            ImGui::Text("Left Ctrl to move Down");
            ImGui::Text("ArrowLeft to look left");
            ImGui::Text("ArrowUp to look up");
            ImGui::Text("ArrowDown to look down");
            ImGui::Text("ArrowRight to look right");
            ImGui::TreePop();
        }
        ImGui::End();

        // Settings panel
        ImGui::Begin("Settings");

        ImGui::Text("Camera Settings");

        if (ImGui::SliderFloat("Camera focal", &focal, 0.001, 50))
        {
            camera.setFocal(focal);
            renderer.resetAcc();
        }
        if (ImGui::DragFloat3("Camera position", glm::value_ptr(camera.position)))
        {
            renderer.resetAcc();
        }
        glm::vec3 lookAt = camera.getLookAt();
        if (ImGui::DragFloat3("Camera lookAt", glm::value_ptr(lookAt)))
        {
            camera.setLookAt(lookAt);
            renderer.resetAcc();
        }


        // cam preset part

        int node_clicked = -1;

        static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

        ImGuiTreeNodeFlags node_flags = base_flags;
        // Items 3..5 are Tree Leaves
        // The only reason we use TreeNode at all is to allow selection of the leaf. Otherwise we can
        // use BulletText() or advance the cursor by GetTreeNodeToLabelSpacing() and call Text().
        ImGui::PushID(0);
        if (ImGui::TreeNodeEx((void *)(intptr_t)0, node_flags, "Cam Presets"))
        {
            node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
            for (int i = 1; i <= 5; i++)
            {
                ImGui::TreeNodeEx((void *)(intptr_t)i, node_flags, "Cam %d", i);
                if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                {
                    node_clicked = i;
                }
            }
            ImGui::TreePop();
        }
        ImGui::PopID();

        handleCamPreset(node_clicked);

        if (ImGui::Button("Reset Accumulation"))
        {
            renderer.resetAcc();
        }

        ImGui::Separator();
        ImGui::Text("Scene Setting");
        ImGui::PushID(1);

        node_flags = base_flags;
        if (ImGui::TreeNodeEx((void *)(intptr_t) 0, node_flags, "Light Attenuation Formula"))
        {
            node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
            for (uint i = 1; i < 4; i++)
            {
                ImGui::TreeNodeEx((void *)(intptr_t)i, node_flags, renderer.getFormulatoString(i), i);
                if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                {
                    renderer.setAttenuationFormula(i);
                    renderer.resetAcc();
                }
            }
            ImGui::TreePop();
        }
        ImGui::PopID();

        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        // Dockspace Viewport
        ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoScrollbar);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

        m_viewportWidth = (uint32_t) ImGui::GetContentRegionAvail().x;
        m_viewportHeight = (uint32_t) ImGui::GetContentRegionAvail().y;

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
                        ImVec2((float) renderer.getWidth(), (float) renderer.getHeight()));
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
        renderer.Render(scene.getScene(), camera);
    }

    void App::keyboardHandler()
    {
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Escape))
        {
            exit(0);
        }

        bool updateRays = false;
        bool resetAcc = false;
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Z))
        {
            camera.forward();
            resetAcc = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_S))
        {
            resetAcc = true;
            camera.backward();
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Q))
        {
            resetAcc = true;
            camera.left();
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_D))
        {
            resetAcc = true;
            camera.right();
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Space))
        {
            resetAcc = true;
            camera.up();
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_LeftCtrl))
        {
            resetAcc = true;
            camera.down();
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_A))
        {
            resetAcc = true;
            camera.rotateClockWise();
            updateRays = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_E))
        {
            resetAcc = true;
            camera.rotateAntiClockWise();
            updateRays = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_UpArrow))
        {
            resetAcc = true;
            camera.lookUp();
            updateRays = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_DownArrow))
        {
            resetAcc = true;
            camera.lookDown();
            updateRays = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_LeftArrow))
        {
            camera.lookLeft();
            resetAcc = true;
            updateRays = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_RightArrow))
        {
            resetAcc = true;
            camera.lookRight();
            updateRays = true;
        }

        if (updateRays)
        {
            camera.updateRay();
        }
        if (resetAcc)
        {            
            renderer.resetAcc();
        }
    }

    void App::handleCamPreset(const int node_clicked)
    {
        switch (node_clicked)
        {
        case 1:
            camera.position = camPos;
            camera.setLookAt(lookAtPos);
            break;
        case 2:
            camera.position = camPos2;
            camera.setLookAt(lookAtPos2);
            break;
        case 3:
            camera.position = camPos3;
            camera.setLookAt(lookAtPos3);
            break;

        case 4:
            camera.position = camPos4;
            camera.setLookAt(lookAtPos4);
            break;

        case 5:
            camera.position = camPos5;
            camera.setLookAt(lookAtPos5);
            break;

        default:
            return;
        }
        renderer.resetAcc();
    }
} // namespace Raytracing