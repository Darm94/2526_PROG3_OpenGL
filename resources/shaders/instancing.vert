#version 460 core

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec2 vert_uv;
// Step 4)
layout (location = 2) in mat4 inst_mvp;
/*
  as if:
   layout (location = 2) in vec4 inst_mvp;
   layout (location = 3) in vec4 inst_mvp;
   layout (location = 4) in vec4 inst_mvp;
   layout (location = 5) in vec4 inst_mvp;
*/

out vec2 vert_uv_out;
out flat int inst_id_out;

// Step 1/2
//uniform mat4 mvp;

// Step 3
//uniform mat4 mvp[4];

void main() 
{
    // Step 1)
    //gl_Position = mvp * vec4(vert_pos, 1.f);
    
    // Step 2)
    //float offset = gl_InstanceID / 2.f;
    //gl_Position = mvp * vec4(vert_pos + offset, 1.f);

    // Step 3)
    //gl_Position = mvp[gl_InstanceID] * vec4(vert_pos, 1.f);

    // Step 4)
    gl_Position = inst_mvp * vec4(vert_pos, 1.f);
    
    vert_uv_out = vert_uv;
    inst_id_out = gl_InstanceID;
}
