#include "DrawEngine.hpp"

#include "Renderer.h"

#include <GL/glew.h>

#include <utility>

DrawObject::DrawObject(ModelObject* modelPtr, std::vector<vec4>& positions, std::vector<unsigned int>& indices,
    std::shared_ptr<Renderer> rendererPtr, vec4 color)
    : modelObject(modelPtr), renderer(rendererPtr), color(color)
{
    ASSERT(modelPtr);
    ASSERT(rendererPtr);

    // Initialize vertex array and vertex buffer
    vao = std::make_unique<VertexArray>();

    vbo = std::make_unique<VertexBuffer>(positions.data(), positions.size() * sizeof(vec4));
    VertexBufferLayout layout;
    layout.Push<float>(4);
    vao->AddBuffer(*vbo, layout);

    //ibo = std::make_unique<IndexBuffer>(indices.data(), indices.size());
    ibo = std::make_unique<IndexBuffer>(indices.data(), indices.size());

    // Initialize shader
    //shader = std::make_unique<Shader>("res/shaders/Basic.shader");
    shader = std::make_unique<Shader>("res/shaders/Tutorial.shader");

    proj = ScaleMatrix({ 1.0f, 1.0f, 1.0f });
    view = proj;
}

DrawObject::~DrawObject()
{
}

DrawObject::DrawObject(DrawObject&& o) :
    vao(std::move(o.vao)), vbo(std::move(o.vbo)),
    ibo(std::move(o.ibo)), shader(std::move(o.shader)),
    modelObject(std::move(o.modelObject)),
    proj(o.proj), view(o.view), renderer(o.renderer), color(o.color)
{
}

std::unique_ptr<VertexArray> vao;
std::unique_ptr<VertexBuffer> vbo;
std::unique_ptr<IndexBuffer> ibo;
std::unique_ptr<Shader> shader;

mat4 proj, view;

ModelObject* modelObject;
std::shared_ptr<Renderer> renderer;
void DrawObject::Draw()
{
    mat4 model = modelObject->GetModel();

    mat4 MVP = proj * view * model;
    shader->Bind();
    shader->SetUniformMat4f("MVP", MVP);
    shader->SetUniform4f("u_Color", color.x, color.y, color.z, color.w);
    // TODO: COLOR shader->SetUniform4f("", COLOR?);

    renderer->Draw(*vao, *ibo, *shader);
}

DrawEngine::DrawEngine()
{
    GLCALL(glEnable(GL_BLEND));
    GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    renderer = std::make_shared<Renderer>();
}

DrawEngine::~DrawEngine()
{
}

void DrawEngine::RegisterObject(ModelObject* modelPtr, std::vector<vec4>& positions, std::vector<unsigned int>& indices, vec4 color)
{
    objects.emplace_back(modelPtr, positions, indices, renderer, color);
}

void DrawEngine::Draw()
{
    renderer->Clear();
    for (size_t i = 0; i < objects.size(); i++)
    {
        objects[i].Draw();
    }
}