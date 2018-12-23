#pragma once

#include "framework.h"

#include "ModelEngine.hpp"

#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"

#include <vector>
#include <memory>

class DrawObject
{
public:
    DrawObject(ModelObject* modelPtr, std::vector<vec4>& positions, std::vector<unsigned int>& indices,
        std::shared_ptr<Renderer> rendererPtr, vec4 color);
    ~DrawObject();
    DrawObject(DrawObject&& o);

    void Draw();

private:
    std::unique_ptr<VertexArray> vao;
    std::unique_ptr<VertexBuffer> vbo;
    std::unique_ptr<IndexBuffer> ibo;
    std::unique_ptr<Shader> shader;

    vec4 color;
    mat4 proj, view;
    ModelObject* modelObject;
    std::shared_ptr<Renderer> renderer;
};

class DrawEngine
{
public:
    DrawEngine();
    ~DrawEngine();

    void RegisterObject(ModelObject* modelPtr, std::vector<vec4>& positions, std::vector<unsigned int>& indices,
        vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f });
    void Draw();

private:
    std::vector<DrawObject> objects;
    std::shared_ptr<Renderer> renderer;

};