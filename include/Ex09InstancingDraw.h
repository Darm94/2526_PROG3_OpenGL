#pragma once
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>

struct QuadInstance {
    glm::vec3 Position;
    float RotationSpeed;
};

class Ex09InstancingDraw 
{
public:
    Ex09InstancingDraw();
    ~Ex09InstancingDraw();
    void Update(float InDeltaTime);
private:
    class OGLProgram* Program;
    GLuint Vao;
    GLuint Vbo;
    GLuint Ebo;
    class OGLTexture* SmileTexture;
    class OGLTexture* BoxTexture;

    glm::mat4 View;
    glm::mat4 Projection;

    std::vector<QuadInstance> Quads;
    GLuint MvpVbo;
    std::vector<glm::mat4> MvpData;
};
