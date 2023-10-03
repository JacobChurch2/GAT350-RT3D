#include "World01.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"

namespace nc
{
    bool World01::Initialize()
    {
        for (int i = 0; i < 10; i++) 
        {
            m_positions.push_back({ randomf(-1,1), randomf(-1, 1) });
        }
        return true;
    }

    void World01::Shutdown()
    {
    }

    void World01::Update(float dt)
    {
        m_angle += 90 * dt;
        for (int i = 0; i < 10; i++) {
            m_positions[i].x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? -dt : 0;
            m_positions[i].x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? +dt : 0;
            m_positions[i].y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? +dt : 0;
            m_positions[i].y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? -dt : 0;
        }
        m_time += dt;
    }

    void World01::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();

        for (int i = 0; i < 10; i++)
        {
            // render
            glPushMatrix();
            glTranslatef(m_positions[i].x, m_positions[i].y, 0);
            //glRotatef(m_angle, 0, 0, 1);
            glScalef((sin(m_time) + 1) * 0.5, 1, 1);

            glBegin(GL_TRIANGLES);

            glColor3f(1, 0, 0);
            glVertex2f(-0.5f, -0.5f);

            glColor3f(0, 1, 0);
            glVertex2f(0.0f, 0.5f);

            glColor3f(0, 0, 1);
            glVertex2f(0.5f, -0.5f);

            glEnd();

            glPopMatrix();

        }
        // post-render
        renderer.EndFrame();
    }
}
