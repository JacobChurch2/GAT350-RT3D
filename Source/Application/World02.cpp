#include "World02.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"

//#define INTERLEAVE 
#define INDEX

namespace nc
{
    bool World02::Initialize()
    {
        //shaders 
        const char* vertexShader =
            "#version 430\n"
            "layout (location=0) in vec3 position;"
            "layout (location=1) in vec3 color;"
            "layout (location=0) out vec3 ocolor;"
            "void main() {"
            "  ocolor = color;"
            "  gl_Position = vec4(position, 1.0);"
            "}";

        const char* fragmentShader =
            "#version 430\n"
            "layout (location=0) in vec3 color;"
            "out vec4 ocolor;"
            "void main() {"
            "  ocolor = vec4(color, 1);"
            "}";

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertexShader, NULL);
        glCompileShader(vs);

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragmentShader, NULL);
        glCompileShader(fs);

        GLuint program = glCreateProgram();
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glUseProgram(program);

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

#elif defined(INDEX)
        //data
        const float vertexData[] = {
            -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // top-left
             1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
             1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
            -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f  // bottom-left
        };

        GLuint indices[] =
        {
            0, 1, 2,
            2, 3, 0
        };

        //sending data
        //vertex buffer object
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

        //index buffer object
        GLuint ibo;
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        //vertex array object
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glBindVertexBuffer(0, vbo, 0, 6 * sizeof(GLfloat));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

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

    void World02::Shutdown()
    {
    }

    void World02::Update(float dt)
    {
    }

    void World02::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();

        // render
        glBindVertexArray(m_vao);

#ifdef INDEX
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
#else
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

     /*   glBindVertexArray(m_vao2);
        glDrawArrays(GL_TRIANGLES, 0, 3);*/
#endif

        // post-render
        renderer.EndFrame();
    }
}
