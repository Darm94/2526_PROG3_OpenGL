#pragma once
#include <glad/gl.h>

class Ex08PhongDraw 
{
public:
    Ex08PhongDraw();
    ~Ex08PhongDraw();
    void Update(float InDeltaTime);
private:
    class OGLProgram* Program;
    GLuint Vao;
    GLuint Vbo;
    class OGLTexture* BoxTexture;
};
