#pragma once
#include <glad/gl.h>

class Ex05QuadTextureDraw 
{
public:
    Ex05QuadTextureDraw();
    ~Ex05QuadTextureDraw();
    void Update(float InDeltaTime);
private:
    class OGLProgram* Program;
    GLuint Vao;
    GLuint Vbo;
    GLuint Ebo;
    //GLuint SmileTextureId;
    //GLuint BoxTextureId;
    class OGLTexture* SmileTexture;
    class OGLTexture* BoxTexture;
    float mix_factor;
};
