#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include <GL/glew.h>
#include <iostream>

#define ASSERT(x) if(!(x)) __debugbreak();

#define FILE_ASSERT(x) std::ifstream infile(x.c_str());\
    ASSERT(infile.good());

#define GLCALL(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__));

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);


class Renderer
{
private:

public:
    Renderer();
    ~Renderer();

    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};