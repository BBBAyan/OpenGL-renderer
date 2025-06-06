#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <assimp/>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


#define ASSERT(x) if(!(x)) __debugbreak();
    #ifdef _DEBUG
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
    #define GLCall(x) x
#endif

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
    void Clear() const;
    void SetClearColor() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};