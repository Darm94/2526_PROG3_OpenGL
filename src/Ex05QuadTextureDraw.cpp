#include "Ex05QuadTextureDraw.h"
#include <vector>
#include "OGLProgram.h"
#include <cmath>

struct XColor {
    float R;
    float G;
    float B;
    float A;
};

Ex05QuadTextureDraw::Ex05QuadTextureDraw()
{
    Program = new OGLProgram("resources/shaders/quadtexture.vert", "resources/shaders/quadtexture.frag");

    std::vector<float> Vertices = {
        // Positions        // Colors
        -0.5f, -0.5f, 0.f,   1.f, 0.f, 0.f,   // bottom-left
         0.5f, -0.5f, 0.f,   0.f, 1.f, 0.f,   // bottom-right
         0.5f,  0.5f, 0.f,   0.f, 0.f, 1.f,   // top-right
        -0.5f,  0.5f, 0.f,   1.f, 1.f, 0.f    // top-left 
    };

    std::vector<uint32_t> Indexes = {
        0, 1, 2, // Triangle Right
        0, 2, 3  // Triangle Left
    };

    //1. Create VAO
    glGenVertexArrays(1, &Vao);
    glBindVertexArray(Vao);

    //2. Create VBO to load data
    glGenBuffers(1, &Vbo);
    glBindBuffer(GL_ARRAY_BUFFER, Vbo);

    size_t DataSize = Vertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, DataSize, Vertices.data(), GL_STATIC_DRAW);

    //3. Link to Vertex Shader
    GLuint Location_0 = 0;
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);

    GLuint Location_1 = 1;
    glVertexAttribPointer(Location_1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(Location_1);

    //4. Create EBO
    glGenBuffers(1, &Ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indexes.size() * sizeof(uint32_t), Indexes.data(), GL_STATIC_DRAW);

    //5. Set Viewport
    glViewport(0, 0, 800, 600);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    
    Program->Bind();

    GLint BaseColorLocation = glGetUniformLocation(Program->ProgramId, "base_color");
    XColor BaseColor{0.5f, 0.5f, 0.5f, 1.f};
    const GLfloat* BaseColorPtr = reinterpret_cast<GLfloat*>(&BaseColor);
    glUniform4fv(BaseColorLocation, 1, BaseColorPtr);
}

Ex05QuadTextureDraw::~Ex05QuadTextureDraw()
{
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    glDeleteBuffers(1, &Ebo);
    
    delete Program;
}

void Ex05QuadTextureDraw::Update(float InDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT);

    static float ElapsedTime = 0.f;
    ElapsedTime += InDeltaTime;

    XColor TimedColor;
    TimedColor.R = sinf(ElapsedTime) * 0.5f + 0.5;  // [-1; 1] => [0; 1]
    TimedColor.G = cosf(ElapsedTime) * 0.5f + 0.5;  // [-1; 1] => [0; 1]
    TimedColor.B = cosf(ElapsedTime + 1.1f) * 0.5f + 0.5;
    TimedColor.A = 1.f;

    GLint BaseColorLocation = glGetUniformLocation(Program->ProgramId, "base_color");
    glUniform4fv(BaseColorLocation, 1,  reinterpret_cast<GLfloat*>(&TimedColor));

    float ScaleSpeed = 10.f;
    float Scale = sinf(ElapsedTime * 10.f)  * 0.5f + 0.5;
    glUniform1f(glGetUniformLocation(Program->ProgramId, "scale"), Scale);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}
