#include "Ex08PhongDraw.h"
#include <vector>
#include "OGLProgram.h"
#include <cmath>
#include "OGLTexture.h"

Ex08PhongDraw::Ex08PhongDraw()
{
    Program = new OGLProgram("resources/shaders/phong.vert", "resources/shaders/phong.frag");

    // Counter-clock wise. Vertex data are NO MORE in NDC space.
        std::vector<float> Vertices = {
        //Position     //Uvs
        //FRONT FACE
        -1, -1,  1,     0, 0,  // bottom-left
         1, -1,  1,     1, 0,  // bottom-right
         1,  1,  1,     1, 1,  // top-right  
        -1,  1,  1,     0, 1,  // top-left
        -1, -1,  1,     0, 0,  // bottom-left
         1,  1,  1,     1, 1,  // top-right  
        
        // BACK FACE
         1, -1, -1,     0, 0,  // bottom-left
        -1, -1, -1,     1, 0,  // bottom-right
        -1,  1, -1,     1, 1,  // top-right  
         1,  1, -1,     0, 1,  // top-left
         1, -1, -1,     0, 0,  // bottom-left
        -1,  1, -1,     1, 1,  // top-right  
 
        //LEFT FACE
        -1, -1, -1,     0, 0,    //bottom-left
        -1, -1,  1,     1, 0,    //bottom-right
        -1,  1,  1,     1, 1,    //top-right
        -1,  1, -1,     0, 1,    //top-left 
        -1, -1, -1,     0, 0,    //bottom-left
        -1,  1,  1,     1, 1,    //top-right

        //RIGHT FACE      
         1, -1,  1,     0, 0,    //bottom-left
         1, -1, -1,     1, 0,    //bottom-right
         1,  1, -1,     1, 1,    //top-right
         1,  1,  1,     0, 1,    //top-left 
         1, -1,  1,     0, 0,    //bottom-left
         1,  1, -1,     1, 1,    //top-right

        //TOP FACE      
        -1,  1,  1,     0, 0,    //bottom-left
         1,  1,  1,     1, 0,    //bottom-right
         1,  1, -1,     1, 1,    //top-right
        -1,  1, -1,     0, 1,    //top-left 
        -1,  1,  1,     0, 0,    //bottom-left
         1,  1, -1,     1, 1,    //top-right
         
        //BOTTOM FACE
        -1, -1, -1,     0, 0,    //bottom-left
         1, -1, -1,     1, 0,    //bottom-right
         1, -1,  1,     1, 1,    //top-right
        -1, -1,  1,     0, 1,    //top-left 
        -1, -1, -1,     0, 0,    //bottom-left
         1, -1,  1,     1, 1,    //top-right
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
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);

    GLuint Location_1 = 1;
    glVertexAttribPointer(Location_1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(Location_1);

    //4. Set Viewport
    glViewport(0, 0, 800, 600);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    
    Program->Bind();

    //6. Texture Setup
    BoxTexture = new OGLTexture("resources/textures/wood-box.jpg");
    
    // In case of using `uniform sampler2D smile_tex;` (without layout binding)
    //glUniform1i(glGetUniformLocation(Program->ProgramId, "smile_tex"), 0);

    BoxTexture->Bind(GL_TEXTURE0);

    //7. Enable Depth Testing
    glEnable(GL_DEPTH_TEST);

    //8. Enable Cull Face
    glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CCW);  //default
    //glFrontFace(GL_CW);
    
    //glCullFace(GL_BACK); //default
    //glCullFace(GL_FRONT);
}

Ex08PhongDraw::~Ex08PhongDraw()
{
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    delete BoxTexture;
    delete Program;
}

void Ex08PhongDraw::Update(float InDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static float ElapsedTime = 0.f;
    ElapsedTime += InDeltaTime;

    Program->SetUniform("angle", 20.f * ElapsedTime);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}
