#pragma once
#include <string>
#include <glad/gl.h>
#include <glm/glm.hpp>

struct XColor {
    float R;
    float G;
    float B;
    float A;
};

class OGLProgram
{
public:
    OGLProgram(const std::string& InVertexPath, const std::string& InFragmPath);
    ~OGLProgram();
    void Bind();
    void SetUniform(const std::string& InName, float InValue);
    void SetUniform(const std::string& InName, const XColor& InColor);
    void SetUniform(const std::string& InName, const glm::mat4& InMat);
private:
    GLuint ProgramId;
};