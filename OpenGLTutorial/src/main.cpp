#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#include "tests/TestClearColor.h"

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

test::TestClearColor clearcolor_test;

void onInitialize(void)
{
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error!" << std::endl;
    }

    GLCALL(std::cout << glGetString(GL_VERSION) << std::endl;)

    GLCALL(glEnable(GL_BLEND));
    GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


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

    clearcolor_test.OnUpdate(0.0f);
    clearcolor_test.OnRender();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplFreeGLUT_NewFrame();

    clearcolor_test.OnImGuiRender();
    imgui_display_code();

    // Rendering
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);

    glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound, but prefer using the GL3+ code.
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glutSwapBuffers(); // exchange buffers for double buffering
    glutPostRedisplay();

}

void onIdle()
{
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