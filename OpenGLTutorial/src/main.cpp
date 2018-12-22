#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#include <GL/glew.h>
#include <GL/glut.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_freeglut.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// OpenGL documentation: http://docs.gl/

int windowWidth = 1280;
int windowHeight = 720;

float r = 0.0f;

Renderer * renderer;
Shader* shader;
VertexArray* va;
VertexBuffer* vb;
IndexBuffer* ib;
Texture* texture;
glm::mat4 MVP;
glm::mat4 proj;
glm::mat4 view;
glm::vec3 translationA;
glm::vec3 translationB;

void onInitialize(void)
{
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error!" << std::endl;
    }

    GLCALL(std::cout << glGetString(GL_VERSION) << std::endl;)

    float positions[] = {
        -50.0f, -50.0f, 0.0f, 0.0f,
         50.0f, -50.0f, 1.0f, 0.0f,
         50.0f,  50.0f, 1.0f, 1.0f,
        -50.0f,  50.0f, 0.0f, 1.0f
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

    translationA = glm::vec3(200, 200, 0);
    translationB = glm::vec3(400, 200, 0);

    proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0 ,0));

    va->Unbind();
    vb->Unbind();
    ib->Unbind();
    shader->Unbind();

    renderer = new Renderer();

}

void imgui_display_code()
{
    static float f = 0.0f;
    ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);
    ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void onDisplay(void)
{
    renderer->Clear();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplFreeGLUT_NewFrame();

    imgui_display_code();

    // Rendering
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);

    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
        MVP = proj * view * model;
        shader->Bind();
        shader->SetUniformMat4f("u_MVP", MVP);
        renderer->Draw(*va, *ib, *shader);
    }
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
        MVP = proj * view * model;
        shader->Bind();
        shader->SetUniformMat4f("u_MVP", MVP);
        renderer->Draw(*va, *ib, *shader);
    }
    shader->Unbind();

    glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound, but prefer using the GL3+ code.
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glutSwapBuffers(); // exchange buffers for double buffering
    glutPostRedisplay();

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
    // Create GLUT window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    //glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL Tutorial");

    onInitialize();

    glutIdleFunc(onIdle);
    glutDisplayFunc(onDisplay);

    // Setup Dear ImGui context
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

    // Setup Platform/Renderer bindings
    ImGui_ImplFreeGLUT_Init();
    ImGui_ImplFreeGLUT_InstallFuncs();
    ImGui_ImplOpenGL3_Init();

    // Setup Style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    glutMainLoop();

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplFreeGLUT_Shutdown();
    ImGui::DestroyContext();

    delete ib;
    delete vb;
    delete va;
    delete shader;
    delete renderer;
    delete texture;

    return 0;
}