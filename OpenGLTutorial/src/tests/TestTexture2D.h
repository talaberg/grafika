#pragma once

#include "Test.h"

#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "Shader.h"

#include <memory>

namespace test
{
    class TestTexture2D : public Test
    {
    private:
        std::unique_ptr<VertexArray> m_vao;
        std::unique_ptr<VertexBuffer> m_vbo;
        std::unique_ptr<IndexBuffer> m_ibo;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Texture> m_Texture;

        glm::mat4 m_Proj, m_View;
        glm::vec3 m_TranslationA, m_TranslationB;

    public:
        TestTexture2D();
        ~TestTexture2D();

        void OnUpdate(float deltaTime) override;
        void OnRender(Renderer* renderer = nullptr) override;
        void OnImGuiRender() override;

    };
}