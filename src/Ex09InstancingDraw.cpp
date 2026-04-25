#include "Ex09InstancingDraw.h"
#include <vector>
#include "OGLProgram.h"
#include <cmath>
#include "OGLTexture.h"
#include <format>

Ex09InstancingDraw::Ex09InstancingDraw()
{
    Program = new OGLProgram("resources/shaders/instancing.vert", "resources/shaders/instancing.frag");

    std::vector<float> Vertices = {
        // Positions         // Uvs
        -0.5f, -0.5f, 0.f,   0.f, 0.f,   // bottom-left
         0.5f, -0.5f, 0.f,   1.f, 0.f,   // bottom-right
         0.5f,  0.5f, 0.f,   1.f, 1.f,   // top-right
        -0.5f,  0.5f, 0.f,   0.f, 1.f,   // top-left 
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
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);

    GLuint Location_1 = 1;
    glVertexAttribPointer(Location_1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(Location_1);

    //4. Create EBO
    glGenBuffers(1, &Ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indexes.size() * sizeof(uint32_t), Indexes.data(), GL_STATIC_DRAW);

    //5. Set Viewport
    glViewport(0, 0, 800, 600);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    
    Program->Bind();

    //6. Texture Setup
    SmileTexture = new OGLTexture("resources/textures/smile.png");
    BoxTexture = new OGLTexture("resources/textures/wood-box.jpg");

    SmileTexture->Bind(GL_TEXTURE0);
    BoxTexture->Bind(GL_TEXTURE1);

    //7. Enable Alpha Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Camera
    glm::vec3 Position = glm::vec3(0, 0, 8);
    glm::vec3 Direction = glm::vec3(0, 0, -1);
    glm::vec3 Up = glm::vec3(0, 1, 0);
    float FovY = 60.f;
    float AspectRatio = 800.f / 600.f;
    float ZNear = 0.01f;
    float ZFar = 100.f;

    View = glm::lookAt(Position, Position + Direction, Up);
    Projection = glm::perspective(glm::radians(FovY), AspectRatio, ZNear, ZFar);

    Quads.resize(4);
    for(int Index = 0; Index < 4; ++Index) 
    {
        auto& Quad = Quads[Index];
        float scalar = Index - (4.f / 2.f) + 0.5f;
        Quad.Position = glm::vec3(scalar, scalar, 0.f);
        Quad.RotationSpeed = 20.f * (Index + 1);
    }

    // Step 4)
    MvpData.resize(Quads.size());

    glGenBuffers(1, &MvpVbo);
    glBindBuffer(GL_ARRAY_BUFFER, MvpVbo);
    glBufferData(GL_ARRAY_BUFFER, MvpData.size() * sizeof(glm::mat4), NULL, GL_STREAM_DRAW);

    GLuint Location_Mvp = 2;
    glVertexAttribPointer(Location_Mvp, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glEnableVertexAttribArray(Location_Mvp);
    glVertexAttribDivisor(Location_Mvp, 1);

    Location_Mvp++;
    glVertexAttribPointer(Location_Mvp, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(Location_Mvp);
    glVertexAttribDivisor(Location_Mvp, 1);

    Location_Mvp++;
    glVertexAttribPointer(Location_Mvp, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(Location_Mvp);
    glVertexAttribDivisor(Location_Mvp, 1);

    Location_Mvp++;
    glVertexAttribPointer(Location_Mvp, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(Location_Mvp);
    glVertexAttribDivisor(Location_Mvp, 1);
}

Ex09InstancingDraw::~Ex09InstancingDraw()
{
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    glDeleteBuffers(1, &Ebo);

    delete SmileTexture;
    delete BoxTexture;
    delete Program;
}

void Ex09InstancingDraw::Update(float InDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT);

    //Step 1)
    /*
    for (int Index=0; Index < 4; ++Index)
    {
        glm::mat4 Model = glm::mat4(1.f);
        Model = glm::translate(Model, glm::vec3(-3 + Index, -3 + Index, 0));

        glm::mat4 Mvp = Projection * View * Model;
        Program->SetUniform("mvp", Mvp);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
    }
    */

    // Step 2)
    /*
    glm::mat4 Model = glm::mat4(1.f);
    glm::mat4 Mvp = Projection * View * Model;
    Program->SetUniform("mvp", Mvp);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0, 4);
    */

    // Step 3)
    /*
    for (int Index=0; Index < 4; ++Index)
    {
        glm::mat4 Model = glm::mat4(1.f);
        Model = glm::translate(Model, glm::vec3(-3 + Index, -3 + Index, 0));

        glm::mat4 Mvp = Projection * View * Model;
        std::string IndexedUniform = std::format("mvp[{}]", Index);
        Program->SetUniform(IndexedUniform, Mvp);
    }

    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0, 4);
    */

    static float ElapsedTime = 0;
    ElapsedTime += InDeltaTime;

    for (int Index=0; Index < Quads.size(); ++Index)
    {
        auto& Quad = Quads[Index];

        glm::mat4 Model = glm::mat4(1.f);
        Model = glm::translate(Model, Quad.Position);
        Model = glm::rotate(Model, glm::radians(Quad.RotationSpeed * ElapsedTime), glm::vec3(0.f, 0.f, 1.f));

        glm::mat4 Mvp = Projection * View * Model;
        // std::string IndexedUniform = std::format("mvp[{}]", Index);
        //Program->SetUniform(IndexedUniform, Mvp);
        
        //glBindBuffer(MvpVbo);
        //glBufferSubData(GL_ARRAY_BUFFER, Index * sizeof(glm::mat4), sizeof(glm::mat4), &Mvp);

        MvpData[Index] = Mvp;
    }
    // Step 4)
    glBufferSubData(GL_ARRAY_BUFFER, 0, MvpData.size() * sizeof(glm::mat4), MvpData.data());

    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0, Quads.size());

    
}
