#pragma once
#include <glad/gl.h>

class Ex03QuadIndexDraw2 
{
public:
    Ex03QuadIndexDraw2();
    ~Ex03QuadIndexDraw2();
    void Update(float InDeltaTime);
private:
    class OGLProgram* Program;
    GLuint Vao;
    GLuint PosVbo;
    GLuint ColorVbo;
    GLuint Ebo;
};
