#include "Ex03QuadIndexDraw.h"
#include <vector>
#include "OGLProgram.h"

Ex03QuadIndexDraw::Ex03QuadIndexDraw()
{
    Program = new OGLProgram("resources/shaders/triangle.vert", "resources/shaders/triangle.frag");

    std::vector<float> Vertices = {
        /*
        // Triangle Right
        -0.5f, -0.5f, 0.f,    // bottom-left
         0.5f, -0.5f, 0.f,    // bottom-right
         0.5f,  0.5f, 0.f,    // top-right
        
        // Triangle Left
        -0.5f, -0.5f, 0.f,    // bottom-left
         0.5f,  0.5f, 0.f,    // top-right
        -0.5f,  0.5f, 0.f     // top-left  
        */

    /*
        -0.5f, -0.5f, 0.f,    // bottom-left
         0.5f, -0.5f, 0.f,    // bottom-right
         0.5f,  0.5f, 0.f,    // top-right
        -0.5f,  0.5f, 0.f     // top-left 
        */
     -0.5f, -0.5f, 0.f,    1.f, 0.f, 0.f,  // bottom-left,red
     0.5f, -0.5f, 0.f,    0.f, 1.f, 0.f,   // bottom-right,green  
     0.5f,  0.5f, 0.f,    0.f, 0.f, 1.f,   // top-right,blue
     -0.5f,  0.5f, 0.f,    1.f, 1.f, 0.f   // top-left,yellow
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

    //new: color location
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //4. Create EBO
    glGenBuffers(1, &Ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indexes.size() * sizeof(uint32_t), Indexes.data(), GL_STATIC_DRAW);

    //5. Set Viewport
    glViewport(0, 0, 800, 600);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    
    Program->Bind();
}

Ex03QuadIndexDraw::~Ex03QuadIndexDraw()
{
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    glDeleteBuffers(1, &Ebo);
    
    delete Program;
}

void Ex03QuadIndexDraw::Update(float InDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}
