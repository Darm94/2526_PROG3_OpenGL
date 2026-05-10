#include "Ex11DeferredDraw.h"
#include <vector>
#include "OGLProgram.h"
#include <cmath>
#include "OGLTexture.h"
#include "ObjParser.h"
#include <iostream>

Ex11DeferredDraw::Ex11DeferredDraw()
{
    Program = new OGLProgram("resources/shaders/deferred_data.vert", "resources/shaders/deferred_data.frag");

    Obj TrupMesh;
    ObjParser::TryParse("resources/models/stormtrooper.obj", TrupMesh);

    std::vector<float> Vertices;
    for(int Index = 0; Index < TrupMesh.triangles.size(); ++Index)
    {
        auto& triangle = TrupMesh.triangles[Index];

        Vertices.push_back(triangle.v1.point.x);
        Vertices.push_back(triangle.v1.point.y);
        Vertices.push_back(triangle.v1.point.z);
        Vertices.push_back(triangle.v1.uv.x);
        Vertices.push_back(triangle.v1.uv.y);
        Vertices.push_back(triangle.v1.normal.x);
        Vertices.push_back(triangle.v1.normal.y);
        Vertices.push_back(triangle.v1.normal.z);

        Vertices.push_back(triangle.v2.point.x);
        Vertices.push_back(triangle.v2.point.y);
        Vertices.push_back(triangle.v2.point.z);
        Vertices.push_back(triangle.v2.uv.x);
        Vertices.push_back(triangle.v2.uv.y);
        Vertices.push_back(triangle.v2.normal.x);
        Vertices.push_back(triangle.v2.normal.y);
        Vertices.push_back(triangle.v2.normal.z);

        Vertices.push_back(triangle.v3.point.x);
        Vertices.push_back(triangle.v3.point.y);
        Vertices.push_back(triangle.v3.point.z);
        Vertices.push_back(triangle.v3.uv.x);
        Vertices.push_back(triangle.v3.uv.y);
        Vertices.push_back(triangle.v3.normal.x);
        Vertices.push_back(triangle.v3.normal.y);
        Vertices.push_back(triangle.v3.normal.z);
    }

    TrupVertexCount = TrupMesh.triangles.size() * 3;


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
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);

    GLuint Location_1 = 1;
    glVertexAttribPointer(Location_1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(Location_1);

    GLuint Location_2 = 2;
    glVertexAttribPointer(Location_2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(Location_2);

    //4. Set Viewport
    glViewport(0, 0, 800, 600);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    
    //Program->Bind();

    //6. Texture Setup
    TrupTexture = new OGLTexture("resources/models/stormtrooper.png");
    //TrupTexture->Bind(GL_TEXTURE0);

    //7. Enable Depth Testing
    //glEnable(GL_DEPTH_TEST);

    //8. Enable Cull Face
    //glEnable(GL_CULL_FACE);

    // Camera
    glm::vec3 Position = glm::vec3(0, 0, 8);
    glm::vec3 Direction = glm::vec3(0, 0, -1);
    glm::vec3 Up = glm::vec3(0, 1, 0);
    float FovY = 60.f;
    float AspectRatio = 800.f / 600.f;
    float ZNear = 0.01;
    float ZFar = 100.f;

    View = glm::lookAt(Position, Position + Direction, Up);
    Projection = glm::perspective(glm::radians(FovY), AspectRatio, ZNear, ZFar);


    // Deferred: Prepare Framebuffer for preparing "GBuffer"
    glGenFramebuffers(1, &GFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, GFbo);

    //1. Render Target Diffuse (3 x Byte => 8 bit precision) [Color attachment 0]
    glGenTextures(1, &DiffuseTexture);
    glBindTexture(GL_TEXTURE_2D, DiffuseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); // Eventually GL_FLOAT for more precision
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, DiffuseTexture, 0);

    //2. Render Target Normal (3 x Float => 16 bit precision) [Color attachment 1]
    glGenTextures(1, &NormalTexture);
    glBindTexture(GL_TEXTURE_2D, NormalTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, NormalTexture, 0);

    //3. Render Target Position (3 x Float => 16 bit precision) [Color attachment 2]
    glGenTextures(1, &PositionTexture);
    glBindTexture(GL_TEXTURE_2D, PositionTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, PositionTexture, 0);

    GLenum Attachs[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, Attachs);

    //4. Attach Depth to framebuffer
    glGenRenderbuffers(1, &DepthRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, DepthRbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 800, 600);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, 0); //unbind current render buffer

    bool rboSuccess = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    if (!rboSuccess) {
        std::cout << "Error creating the framebuffer\n";
        throw std::runtime_error("Error creating the framebuffer");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind and back to swapchain framebuffer

    // Blending pass pipeline

    BlendProgram = new OGLProgram("resources/shaders/deferred_blending.vert", "resources/shaders/deferred_blending.frag");

    std::vector<float> BlendVertices = {
        // Positions      // Uvs
        -1.f, -1.f,       0.f, 0.f,   // bottom-left
         1.f, -1.f,       1.f, 0.f,   // bottom-right
        -1.f,  1.f,       0.f, 1.f,   // top-left 

        -1.f,  1.f,       0.f, 1.f,   // top-left 
         1.f, -1.f,       1.f, 0.f,   // bottom-right
         1.f,  1.f,       1.f, 1.f,   // top-right
    };

    //1. Create VAO
    glGenVertexArrays(1, &BlendVao);
    glBindVertexArray(BlendVao);

    //2. Create VBO to load data
    glGenBuffers(1, &BlendVbo);
    glBindBuffer(GL_ARRAY_BUFFER, BlendVbo);

    size_t BlendDataSize = BlendVertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, BlendDataSize, BlendVertices.data(), GL_STATIC_DRAW);

    //3. Link to Vertex Shader
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // LIGHT Configuration
    BlendProgram->Bind();
    BlendProgram->SetUniform("point_light_poses[0]", glm::vec3(4, 0, 0));
    BlendProgram->SetUniform("point_light_poses[1]", glm::vec3(-4, 0, 0));
    BlendProgram->SetUniform("point_light_poses[2]", glm::vec3(0, 4, 0));
}

Ex11DeferredDraw::~Ex11DeferredDraw()
{
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    delete TrupTexture;
    delete Program;

    glDeleteFramebuffers(1, &GFbo);
    glDeleteTextures(1, &DiffuseTexture);
    glDeleteTextures(1, &NormalTexture);
    glDeleteTextures(1, &PositionTexture);
    glDeleteRenderbuffers(1, &DepthRbo);

    glDeleteVertexArrays(1, &BlendVao);
    glDeleteBuffers(1, &BlendVbo);
    delete BlendProgram;
}

static void DebugGBuffer(GLuint InFbo)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);    // GL_FRAMEBUFFER = Read + Write operation
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, InFbo); // GL_READ_FRAMEBUFFER = Read operation

    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glBlitFramebuffer(
        0, 0, 800, 600,    // Source Bounds
        0, 0, 400, 300,    // Destin Bounds (BOTTOM-LEFT)
        GL_COLOR_BUFFER_BIT,  // Which buffer to write to
        GL_LINEAR          // interpolation in case of streching image (linear or nearest)
    );

    glReadBuffer(GL_COLOR_ATTACHMENT1); 
    glBlitFramebuffer(
        0, 0, 800, 600,    // Source Bounds
        400, 0, 800, 300,    // Destin Bounds (BOTTOM-RIGHT)
        GL_COLOR_BUFFER_BIT,  // Which buffer to write to
        GL_LINEAR          // interpolation in case of streching image (linear or nearest)
    );

    glReadBuffer(GL_COLOR_ATTACHMENT2);
    glBlitFramebuffer(
        0, 0, 800, 600,    // Source Bounds
        0, 300, 400, 600,    // Destin Bounds (TOP-LEFT)
        GL_COLOR_BUFFER_BIT,  // Which buffer to write to
        GL_LINEAR          // interpolation in case of streching image (linear or nearest)
    );
}

void Ex11DeferredDraw::Update(float InDeltaTime)
{
    // Geometry Pass
    glBindFramebuffer(GL_FRAMEBUFFER, GFbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    static float ElapsedTime = 0.f;
    ElapsedTime += InDeltaTime;
    
    float Angle = 20.f * ElapsedTime;

    glm::mat4 Model = glm::mat4(1.f);
    Model = glm::translate(Model, glm::vec3(0, -4, 0));
    Model = glm::rotate(Model, glm::radians(-Angle), glm::vec3(0, 1, 0));
    Model = glm::scale(Model, glm::vec3(2.f));

    glm::mat4 Mvp = Projection * View * Model;

    Program->Bind();
    Program->SetUniform("mvp", Mvp);
    Program->SetUniform("model", Model);

    glBindVertexArray(Vao);
    TrupTexture->Bind(GL_TEXTURE0);
    glDrawArrays(GL_TRIANGLES, 0, TrupVertexCount);

    //DebugGBuffer(GFbo);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    BlendProgram->Bind();
    glBindVertexArray(BlendVao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, DiffuseTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, NormalTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, PositionTexture);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}
