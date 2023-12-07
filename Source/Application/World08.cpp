#include "World08.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>

#define INTERLEAVE 


namespace nc
{
    bool World08::Initialize()
    {
        m_scene = std::make_unique<Scene>();
        m_scene->Load("Scenes/scene_editor.json");
        m_scene->Load("Scenes/scene_cel.json");
        m_scene->Initialize();

        for (int i = 0; i < 500; i++) {

            int minx = randomf(10, 20);
            int maxx = randomf(30, 40);
            int minz = randomf(10, 15);
            int maxz = randomf(20, 25);
            auto actor = CREATE_CLASS(Actor);
            actor->name = StringUtils::CreateUnique("Mushroom");
            actor->active = true;
            actor->transform.position = glm::vec3{ randomf(minx, maxx), 0, -random(minz, maxz) };
            actor->transform.scale = glm::vec3{ 0.5, 0.5, 0.5 };

            auto model = CREATE_CLASS(ModelComponent);
            model->modelName = "fantasy/obj/SF_Env_Mushroom_01.obj";
            model->materialName = "fantasy/fantasy.mtrl";

            actor->AddComponent(std::move(model));

            actor->Initialize();
            m_scene->Add(std::move(actor));
        }

        for (int i = 0; i < 200; i++) {

            int minx = randomf(18, 28);
            int maxx = randomf(30, 40);
            int maxz = randomf(-5, -1);
            auto actor = CREATE_CLASS(Actor);
            actor->name = StringUtils::CreateUnique("Mushroom");
            actor->active = true;
            actor->transform.position = glm::vec3{ randomf(minx, maxx), 0, randomf(-10, maxz)};
            actor->transform.scale = glm::vec3{ 0.5, 0.5, 0.5 };

            auto model = CREATE_CLASS(ModelComponent);
            model->modelName = "fantasy/obj/SF_Env_Mushroom_01.obj";
            model->materialName = "fantasy/fantasy.mtrl";

            actor->AddComponent(std::move(model));

            actor->Initialize();
            m_scene->Add(std::move(actor));
        }

        m_editor = std::make_unique<Editor>();

        //create depth texture
        auto texture = std::make_shared<Texture>();
        texture->CreateDepthTexture(1024, 1024);
        ADD_RESOURCE("depth_texture", texture);

        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateDepthBuffer(texture);
        ADD_RESOURCE("depth_buffer", framebuffer);

        //set depth texture to debug sprite
        auto material = GET_RESOURCE(Material, "materials/sprite.mtrl");
        if (material)
        {
            material->albedoTexture = texture;
        }

        auto materials = GET_RESOURCES(Material);
        for (auto material : materials)
        {
            material->depthTexture = texture;
        }

        return true;
    }

    void World08::Shutdown()
    {
    }

    void World08::Update(float dt)
    {
        //GUI!!!!!
        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);

        m_editor->Update();
        m_editor->ProcessGui(m_scene.get());

        ImGui::Begin("Cel");
        ImGui::SliderInt("Levels", &m_celLevels, 1, 8);
        ImGui::SliderFloat("Specualr Cutoff", &m_celSpecularCutoff, 0, 1);
        ImGui::SliderFloat("Outline", &m_celOutline, 0, 1);
        ImGui::End();

        auto program = GET_RESOURCE(Program, "shaders/lit_phong_cel.prog");
        if (program)
        {
            program->Use();
            program->SetUniform("celLevels", m_celLevels);
            program->SetUniform("celSpecularCutoff", m_celSpecularCutoff);
            program->SetUniform("celOutline", m_celOutline);
        }

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World08::Draw(Renderer& renderer)
    {
        // *** pass 1 ***
        auto framebuffer = GET_RESOURCE(Framebuffer, "depth_buffer");
        renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();

        renderer.ClearDepth();
        auto program = GET_RESOURCE(Program, "shaders/shadow_depth.prog");
        program->Use();

        auto lights = m_scene->GetComponents<LightComponent>();
        for (auto light : lights)
        {
            if (light->castShadow) 
            {
                glm::mat4 shadowMatrix = light->GetShadowMatrix();
                program->SetUniform("shadowVP", shadowMatrix);
            }
        }

        auto models = m_scene->GetComponents<ModelComponent>();
        for (auto model : models) 
        {
            if (model->castShadow) {
                //glCullFace(GL_FRONT);
                program->SetUniform("model", model->m_owner->transform.GetMatrix());
                model->model->Draw();
            }
        }

        framebuffer->Unbind();
        

        // *** pass 2 ***
        renderer.ResetViewport();
        renderer.BeginFrame();
        m_scene->Draw(renderer);

        // post-render
        ENGINE.GetSystem<Gui>()->Draw();
        renderer.EndFrame();
    }
}
