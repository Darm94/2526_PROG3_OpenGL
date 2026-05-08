#pragma once
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Ex11DeferredDraw 
{
public:
    Ex11DeferredDraw();
    ~Ex11DeferredDraw();
    void Update(float InDeltaTime);
private:
    class OGLProgram* Program;
    GLuint Vao;
    GLuint Vbo;
    class OGLTexture* TrupTexture;
    size_t TrupVertexCount;
    glm::mat4 View;
    glm::mat4 Projection;

    GLuint GFbo;
    GLuint DiffuseTexture;
    GLuint NormalTexture;
    GLuint PositionTexture;
    GLuint DepthRbo;

    class OGLProgram* BlendProgram;
    GLuint BlendVao;
    GLuint BlendVbo;
};
