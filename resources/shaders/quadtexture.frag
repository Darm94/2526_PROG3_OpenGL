#version 460 core

in vec2 vert_uv_out;

//uniform sampler2D smile_tex;
layout (binding = 0) uniform sampler2D smile_tex;
layout (binding = 1) uniform sampler2D box_tex;

uniform float mix_factor;

out vec4 frag_color;

void main() 
{
    //frag_color = vec4(1.f, 0.f, 0.f, 1.f);
    //frag_color = texture(smile_tex, vert_uv_out);

    vec4 smile_texel = texture(smile_tex, vert_uv_out);
    vec4 box_texel = texture(box_tex, vert_uv_out);
    
    //frag_color = mix(smile_texel, box_texel, 0.5f);

    //frag_color = mix( box_texel,smile_texel, step(0.01,smile_texel.a)*0.5f);
    //still mix factor smile on box
    frag_color = mix( box_texel,smile_texel, step(0.01,smile_texel.a)*(1-mix_factor));
/*
    if (smile_texel.a < 0.01)
    {
        frag_color = box_texel;
    }
    else
    {
         frag_color = mix(smile_texel, box_texel, 0.5f);

    }
*/
    
}