#version 460 core

in vec2 vert_uv_out;
in flat int inst_id_out;

layout (binding = 0) uniform sampler2D smile_tex;
layout (binding = 1) uniform sampler2D box_tex;

out vec4 frag_color;

void main() 
{
    vec4 smile_texel = texture(smile_tex, vert_uv_out);
    vec4 box_texel = texture(box_tex, vert_uv_out);

    if (inst_id_out % 2 == 0)
    {
        frag_color = smile_texel;
    } 
    else 
    {
        frag_color = box_texel;
    }
}