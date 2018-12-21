#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#include <GL/glew.h>
#include <GL/glut.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// OpenGL documentation: http://docs.gl/

int windowHeight = 600;
int windowWidth  = 600;

float r = 0.0f;

Renderer * renderer;
Shader* shader;
VertexArray* va;
VertexBuffer* vb;
IndexBuffer* ib;
Texture* texture;
glm::mat4 MVP;

void onInitialize(void)
{
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error!" << std::endl;
    }

    GLCALL(std::cout << glGetString(GL_VERSION) << std::endl;)

    float positions[] = {
        100.0f, 100.0f, 0.0f, 0.0f,
        200.0f, 100.0f, 1.0f, 0.0f,
        200.0f, 200.0f, 1.0f, 1.0f,
        100.0f, 200.0f, 0.0f, 1.0f
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    GLCALL(glEnable(GL_BLEND));
    GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    va = new VertexArray();
    vb = new VertexBuffer(positions, 4 * 4 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);

    va->AddBuffer(*vb, layout);

    ib = new IndexBuffer(indices, 6);

    texture = new Texture("res/textures/chutulu.png");
    texture->Bind(0);

    shader = new Shader("res/shaders/Basic.shader");
    shader->Bind();
    shader->SetUniform1i("u_Texture", 0);

    glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0 ,0));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));;
    MVP = proj * view * model;

    va->Unbind();
    vb->Unbind();
    ib->Unbind();
    shader->Unbind();

    renderer = new Renderer();

}

void onDisplay(void)
{
    renderer->Clear();

    shader->Bind();
    shader->SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
    shader->SetUniformMat4f("u_MVP", MVP);

    renderer->Draw(*va, *ib, *shader);

    glutSwapBuffers(); // exchange buffers for double buffering
}

void onIdle()
{
    static long last_time = 0;
    static long dt = 0;
    long time = glutGet(GLUT_ELAPSED_TIME); // elapsed time since the start of the program
    dt += time - last_time;
    last_time = time;

    if (dt > 100)
    {
        static float increment = 0.05f;
        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;

        r += increment;

        dt = 0;
        glutPostRedisplay();                    // redraw the scene
    }

}

int main(int argc, char** argv)
{


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(windowHeight, windowWidth);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL Tutorial");

    onInitialize();

    glutIdleFunc(onIdle);
    glutDisplayFunc(onDisplay);
    glutMainLoop();

    delete ib;
    delete vb;
    delete va;
    delete shader;
    delete renderer;
    delete texture;

    return 0;
}