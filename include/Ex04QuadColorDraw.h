#pragma once
#include <glad/gl.h>

class Ex04QuadColorDraw 
{
public:
    Ex04QuadColorDraw();
    ~Ex04QuadColorDraw();
    void Update(float InDeltaTime);
private:
    class OGLProgram* Program;
    GLuint Vao;
    GLuint Vbo;
    GLuint Ebo;
};
