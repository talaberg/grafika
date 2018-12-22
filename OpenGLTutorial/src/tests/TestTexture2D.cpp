#include "TestTexture2D.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"

namespace test
{
    TestTexture2D::TestTexture2D()
        : m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0),
        m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
    {
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

        // Initialize vertex array and vertex buffer
        m_vao = std::make_unique<VertexArray>();
        m_vbo = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        m_vao->AddBuffer(*m_vbo, layout);

        // Initialize index array
        m_ibo = std::make_unique<IndexBuffer>(indices, 6);

        // Initialize texture
        m_Texture = std::make_unique<Texture>("res/textures/chutulu.png");
        m_Texture->Bind(0);

        // Initialize shader
        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        m_Shader->Bind();
        m_Shader->SetUniform1i("u_Texture", 0);
    }

    TestTexture2D::~TestTexture2D()
    {
    }

    void TestTexture2D::OnUpdate(float deltaTime)
    {
    }

    void TestTexture2D::OnRender(Renderer* renderer)
    {
        if (renderer)
        {
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
                glm::mat4 MVP = m_Proj * m_View * model;
                m_Shader->Bind();
                m_Shader->SetUniformMat4f("u_MVP", MVP);
                renderer->Draw(*m_vao, *m_ibo, *m_Shader);
            }
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
                glm::mat4 MVP = m_Proj * m_View * model;
                m_Shader->Bind();
                m_Shader->SetUniformMat4f("u_MVP", MVP);
                renderer->Draw(*m_vao, *m_ibo, *m_Shader);
            }
        }
    }

    void TestTexture2D::OnImGuiRender()
    {
        ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}