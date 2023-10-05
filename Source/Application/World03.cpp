#include "World03.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"

#define INTERLEAVE 


namespace nc
{
    bool World03::Initialize()
    {
        m_program = GET_RESOURCE(Program, "Shaders/unlit_color.prog");
        m_program->Use();

#ifdef INTERLEAVE
        //data
        float vertexData[] = {
           -0.8f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f,
           -0.8f,  0.8f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.8f, -0.8f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.8f,  0.8f, 0.0f, 1.0f, 1.0f, 1.0f
        };

        //sending data
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glBindVertexBuffer(0, vbo, 0, 6 * sizeof(GLfloat));

        //position
        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(0, 0);

        //color
        glEnableVertexAttribArray(1);
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));
        glVertexAttribBinding(1, 0);

#else 

        //data
        float positionData[] = {
           -0.8f, -0.8f, 0.0f,
           -0.8f,  0.8f, 0.0f,
            0.8f, -0.8f, 0.0f,
            0.8f,  0.8f, 0.0f
        };

        float colorData[] =
        {
            1.0f, 0.0f, 0.25f,
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.25f,
            0.0f, 0.0f, 0.0f
        };

        /*float positionData2[] = {
           -0.8f, -0.8f, 0.0f,
           -0.8f,  0.8f, 0.0f,
            0.8f,  0.8f, 0.0f
        };

        float colorData2[] =
        {
            0.25f, 0.0f, 1.0f,
            0.0f, 0.0f, 0.0f,
            0.25f, 0.0f, 0.0f
        };*/

        //sending data
        GLuint vbo[2];
        glGenBuffers(2, vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(positionData), positionData, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        //position
        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
        glBindVertexBuffer(0, vbo[0], 0, 3 * sizeof(GLfloat));
        glVertexAttribBinding(0, 0);

        //color
        glEnableVertexAttribArray(1);
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
        glBindVertexBuffer(1, vbo[1], 0, 3 * sizeof(GLfloat));
        glVertexAttribBinding(1, 1);

        ////sending data 2
        //GLuint vbo2[2];
        //glGenBuffers(2, vbo2);
        //glBindBuffer(GL_ARRAY_BUFFER, vbo2[0]);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(positionData2), positionData2, GL_STATIC_DRAW);

        //glBindBuffer(GL_ARRAY_BUFFER, vbo2[1]);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(colorData2), colorData2, GL_STATIC_DRAW);

        //glGenVertexArrays(1, &m_vao2);
        //glBindVertexArray(m_vao2);

        ////position
        //glEnableVertexAttribArray(0);
        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        //glBindVertexBuffer(0, vbo2[0], 0, 3 * sizeof(GLfloat));

        ////color
        //glEnableVertexAttribArray(1);
        //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        //glBindVertexBuffer(1, vbo2[1], 0, 3 * sizeof(GLfloat));

#endif

        return true;
    }

    void World03::Shutdown()
    {
    }

    void World03::Update(float dt)
    {
        m_time += dt;

        GLint uniform = glGetUniformLocation(m_program->m_program, "time");
        glUniform1f(uniform, m_time);
    }

    void World03::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();

        // render
        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

     /*   glBindVertexArray(m_vao2);
        glDrawArrays(GL_TRIANGLES, 0, 3);*/

        // post-render
        renderer.EndFrame();
    }
}
