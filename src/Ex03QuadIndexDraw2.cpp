#include "Ex03QuadIndexDraw2.h"
#include <vector>
#include "OGLProgram.h"

Ex03QuadIndexDraw2::Ex03QuadIndexDraw2()
{
    Program = new OGLProgram("resources/shaders/triangle.vert", "resources/shaders/triangle.frag");

    std::vector<float> Vertices = {

     -0.5f, -0.5f, 0.f, // bottom-left
     0.5f, -0.5f, 0.f,  // bottom-right
     0.5f,  0.5f, 0.f,  // top-right
     -0.5f,  0.5f, 0.f,  // top-left
    };

    std::vector<float> Colors = {
    1.f, 0.f, 0.f,   // bottom-left
    0.f, 1.f, 0.f,   // bottom-right
    0.f, 0.f, 1.f,   // top-right
    1.f, 1.f, 0.f    // top-left
    };

    std::vector<uint32_t> Indexes = {
        0, 1, 2, // Triangle Right
        0, 2, 3  // Triangle Left
    };

    //1. Create VAO
    glGenVertexArrays(1, &Vao);
    glBindVertexArray(Vao);

    //2a. Create VBO  to load POSITIONS data
    glGenBuffers(1, &PosVbo);
    glBindBuffer(GL_ARRAY_BUFFER, PosVbo);
    size_t DataSize = Vertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, DataSize, Vertices.data(), GL_STATIC_DRAW);
    //3a. Link to Vertex Shader
    GLuint Location_0 = 0;
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);

    //2ba. Create VBO  to load COLORS data
    glGenBuffers(1, &ColorVbo);
    glBindBuffer(GL_ARRAY_BUFFER, ColorVbo);
    DataSize = Colors.size() * sizeof(float) ;
    glBufferData(GL_ARRAY_BUFFER, DataSize, Colors.data(), GL_STATIC_DRAW);
     //3b. Link to Vertex Shader
    GLuint Location_1 = 1;
    glVertexAttribPointer(Location_1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_1);

    //4. Create EBO
    glGenBuffers(1, &Ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indexes.size() * sizeof(uint32_t), Indexes.data(), GL_STATIC_DRAW);

    //5. Set Viewport
    glViewport(0, 0, 800, 600);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    
    Program->Bind();
}

Ex03QuadIndexDraw2::~Ex03QuadIndexDraw2()
{
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &PosVbo);
    glDeleteBuffers(1, &ColorVbo);
    glDeleteBuffers(1, &Ebo);
    
    delete Program;
}

void Ex03QuadIndexDraw2::Update(float InDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}
