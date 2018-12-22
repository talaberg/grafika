#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"

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

test::Test* currentTest;
test::TestMenu* testMenu;

Renderer renderer;

void onInitialize(void)
{
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error!" << std::endl;
    }

    GLCALL(std::cout << glGetString(GL_VERSION) << std::endl;)

    GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    currentTest = nullptr;
    testMenu = new test::TestMenu(currentTest);
    currentTest = testMenu;

    testMenu->RegisterTest<test::TestClearColor>("Clear Color");
    testMenu->RegisterTest<test::TestTexture2D>("2D Texture");
}

void onDisplay(void)
{
    renderer.Clear();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplFreeGLUT_NewFrame();

    if (currentTest)
    {
        ImGui::Begin("Test");

        currentTest->OnUpdate(0.0f);
        currentTest->OnRender(&renderer);
        if (currentTest != testMenu && ImGui::Button("<-"))
        {
            delete currentTest;
            currentTest = testMenu;
        }

        currentTest->OnImGuiRender();
        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);

    glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound, but prefer using the GL3+ code.
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glutSwapBuffers(); // exchange buffers for double buffering
    glutPostRedisplay();

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

    delete currentTest;
    if (currentTest != testMenu)
    {
        delete testMenu;
    }

    return 0;
}