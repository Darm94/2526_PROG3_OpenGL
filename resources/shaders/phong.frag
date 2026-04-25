#version 460 core

in vec2 vert_uv_out;

layout (binding = 0) uniform sampler2D box_tex;

out vec4 frag_color;

void main() 
{
    frag_color = texture(box_tex, vert_uv_out);
}