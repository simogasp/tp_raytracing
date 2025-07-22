#include "App.hpp"
#include "glm/gtc/type_ptr.hpp"
namespace Raytracing
{
    App::App()
        : camera(), scene()
    {
        // color helper
        const glm::vec3 black(0.f, 0.f, 0.f);
        const glm::vec3 white(1.f, 1.f, 1.f);
        const glm::vec3 red(1.f, 0.f, 1.f);
        const glm::vec3 blue(51.f / 255, 77.f / 255, 1.f);
        const glm::vec3 orange(0.8f, 0.5f, 0.2f);
        const glm::vec3 gray(122.f / 255, 127.f / 255, 128.f / 255);

        // position helper
        const glm::vec3 redPos(-1.f, 0.f, 0.f);
        const glm::vec3 floorPos(0.f, 1001.f, 0.f);
        const glm::vec3 lightPos(1.f, -10.f, -30.f);
        const glm::vec3 glassPos(1.f, 0.f, 0.f);

        // shinyness helper
        const float mat = 0.0f;
        const float midShiny = 0.5f;
        const float shiny = 1.f;

        // roughness helper
        const float fullRoughness = 1.0f;
        const float midRoughness = .5f;
        const float noRoughness = 0.0f;

        // emissionPower helper
        const float noEmissionPower = 0.F;
        const float normalEmissionPower = 1.F;
        const float midEmissionPower = 2.f;
        const float fullEmissionPower = 10.f;

        // refraction index helper
        const float noTranslucid = 0.f;
        const float airTranslucid = 1.f;
        const float waterTranslucid = 1.33f;
        const float plexiGlassTranslucid = 1.5f;



        // materials
        scene.pushMaterial(red, mat, fullRoughness);
        scene.pushMaterial(gray, shiny, midRoughness);
        scene.pushMaterial(orange, orange, mat, fullRoughness, normalEmissionPower, 1000.f, noTranslucid);
        scene.pushMaterial(white,
                white,
                shiny,
                0.0001f,
                0.f,
                0.f,
                plexiGlassTranslucid);
            scene.pushMaterial(white,
                white,
                shiny,
                noRoughness,
                0.f,
                0.f,
                airTranslucid);

        // spheres
        scene.pushSphere(redPos, 1.f, 0);
        scene.pushSphere(floorPos, 1000.f, 1);
        scene.pushSphere(lightPos, 20.f, 2);
        scene.pushSphere(glassPos, 1.f, 3);
        // scene.pushSphere(glassPos, 0.8f, 4);

        // camera
        camera.setCameraPosition(camPos4);
        camera.setLookAt(lookAtPos4);
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

        ImGui::Text("Camera Settings");

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
            for (int i = 1; i < 5; i++)
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
            for (int i = 1; i < 5; i++)
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

    void App::handleCamPreset(const int node_clicked)
    {
        switch (node_clicked)
        {
        case 1:
            camera.setCameraPosition(camPos);
            camera.setLookAt(lookAtPos);
            break;
        case 2:
            camera.setCameraPosition(camPos2);
            camera.setLookAt(lookAtPos2);
            break;
        case 3:
            camera.setCameraPosition(camPos3);
            camera.setLookAt(lookAtPos3);
            break;

        case 4:
            camera.setCameraPosition(camPos4);
            camera.setLookAt(lookAtPos4);
            break;

        default:
            return;
        }
        renderer.resetAcc();
    }
} // namespace Raytracing