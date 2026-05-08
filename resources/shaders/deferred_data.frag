#version 460 core

in vec2 vert_uv_out;
in vec3 vert_world_norm_out;
in vec3 vert_world_pos_out;

layout (binding = 0) uniform sampler2D trup_tex;

out vec3 diffuse_color;  // GL_COLOR_ATTACHMENT0
out vec3 world_normal;   // GL_COLOR_ATTACHMENT1
out vec3 world_position; // GL_COLOR_ATTACHMENT2

void main() 
{
    diffuse_color = texture(trup_tex, vert_uv_out).rgb;
    world_normal = vert_world_norm_out;
    world_position = vert_world_pos_out;
}