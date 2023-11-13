#include "World06.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>

#define INTERLEAVE 


namespace nc
{
    bool World06::Initialize()
    {
        m_scene = std::make_unique<Scene>();
        m_scene->Load("Scenes/scene_framebuffer.json");
        m_scene->Initialize();

        auto texture = std::make_shared<Texture>();
        texture->CreateTexture(1024, 1024);
        ADD_RESOURCE("fb_texture", texture);

        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateFramebuffer(texture);
        ADD_RESOURCE("fb", framebuffer);

        auto material = GET_RESOURCE(Material, "materials/postprocess.mtrl");
        if (material)
        {
            material->albedoTexture = texture;
        }

        return true;
    }

    void World06::Shutdown()
    {
    }

    void World06::Update(float dt)
    {
        //GUI!!!!!
        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);
        m_scene->ProcessGui();

        //set postprocess gui
        ImGui::Begin("Post-Process");
        ImGui::SliderFloat("Blend", &m_blend, 0, 1);
        bool effect = m_params & INVERT_MASK;
        if (ImGui::Checkbox("Invert", &effect)) {
            (effect) ? m_params |= INVERT_MASK : m_params &= ~INVERT_MASK;
        }
        effect = m_params & GRAYSCALE_MASK;
        if (ImGui::Checkbox("Grayscale", &effect)) {
            (effect) ? m_params |= GRAYSCALE_MASK : m_params &= ~GRAYSCALE_MASK;
        }
        effect = m_params & COLORTINT_MASK;
        if (ImGui::Checkbox("ColorTint", &effect)) {
            (effect) ? m_params |= COLORTINT_MASK : m_params &= ~COLORTINT_MASK;
        }
        ImGui::ColorEdit3("Tint", &m_colorTint[0], 0.1f);
        effect = m_params & GRAIN_MASK;
        if (ImGui::Checkbox("Grain", &effect)) {
            (effect) ? m_params |= GRAIN_MASK : m_params &= ~GRAIN_MASK;
        }
        effect = m_params & SCANLINE_MASK;
        if (ImGui::Checkbox("Scanline", &effect)) {
            (effect) ? m_params |= SCANLINE_MASK : m_params &= ~SCANLINE_MASK;
        }
        effect = m_params & EDGE_MASK;
        if (ImGui::Checkbox("Edge", &effect)) {
            (effect) ? m_params |= EDGE_MASK : m_params &= ~EDGE_MASK;
        }
        effect = m_params & BLUR_MASK;
        if (ImGui::Checkbox("Blur", &effect)) {
            (effect) ? m_params |= BLUR_MASK : m_params &= ~BLUR_MASK;
        }
        effect = m_params & CUSTOM_MASK;
        if (ImGui::Checkbox("Custom", &effect)) {
            (effect) ? m_params |= CUSTOM_MASK : m_params &= ~CUSTOM_MASK;
        }
        ImGui::End();

        //set postprocess shader
        auto program = GET_RESOURCE(Program, "shaders/post_process.prog");
        if (program) 
        {
            program->Use();
            program->SetUniform("blend", m_blend);
            program->SetUniform("colorTint", m_colorTint);
            program->SetUniform("params", m_params);
        }

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World06::Draw(Renderer& renderer)
    {
        // *** pass 1 ***
        m_scene->GetActorByName("postprocess")->active = false;

        auto framebuffer = GET_RESOURCE(Framebuffer, "fb");
        renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();

        renderer.BeginFrame({0,0,0});
        m_scene->Draw(renderer);

        framebuffer->Unbind();

        // *** pass 2 ***
        m_scene->GetActorByName("postprocess")->active = true;

        renderer.ResetViewport();
        renderer.BeginFrame();
        m_scene->Draw(renderer);
        m_scene->GetActorByName("postprocess")->Draw(renderer);

        // post-render
        ENGINE.GetSystem<Gui>()->Draw();
        renderer.EndFrame();
    }
}
