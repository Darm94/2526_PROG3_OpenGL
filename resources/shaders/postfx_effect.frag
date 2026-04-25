#version 460 core

in vec2 vert_uv_out;

layout (binding = 0) uniform sampler2D scene_tex;

out vec4 frag_color;

uniform float time;

vec4 blur() {
    //3x3
    //texture(scene_tex, vert_uv_out);

    vec2 tex_size = textureSize(scene_tex, 0);
    float uv_w = 1.f / tex_size.x; //800
    float uv_h = 1.f / tex_size.y; //600

    vec4 contrib = vec4(0.f);
    for(float i=-10.f; i <= 10.f; ++i) 
    {
        for(float j=-10.f; j <= 10.f; ++j) 
        {
            float xoff = uv_w * i;
            float yoff = uv_h * i;
            vec2 uv = vec2(vert_uv_out.x + xoff, vert_uv_out.y + yoff);
            contrib += texture(scene_tex, uv);
        }
    }

    vec4 color = contrib / 400;
    color.a = 1.f;
    return color;
}

vec4 wave() {
    //y = A sin(B(x + C)) + D
    float A = 0.02f;
    float B = 20.f;
    float C = time / 40.f;
    float D = 0.f;
    vec2 uv_curr = vert_uv_out;
    uv_curr.x += A * sin(B * (uv_curr.y + C)) + D;
    return texture(scene_tex, uv_curr);
}

vec4 quake() {
    vec2 uv_curr = vert_uv_out;
    float stregth = 0.01;

    uv_curr.x += cos(time * 30) * stregth;
    uv_curr.y += cos(time * 45) * stregth;
    return texture(scene_tex, uv_curr);
}

void main() 
{
    // As-Is
    //frag_color = texture(scene_tex, vert_uv_out);

    /* Grayscale base 
    vec4 color = texture(scene_tex, vert_uv_out);
    float avg = (color.r + color.g + color.b) / 3.0;
    frag_color = vec4(avg, avg, avg, color.a);
    */

    /* Grayscale weighted 
    vec4 color = texture(scene_tex, vert_uv_out);
    float avgw = (0.3f * color.r + 0.59f * color.g + 0.11f * color.b);
    frag_color = vec4(avgw, avgw, avgw, color.a);
    */

    /* Negative
    vec4 color = texture(scene_tex, vert_uv_out);
    frag_color = 1.f - color;
    */

    /* Blackband (uv based)
    if (vert_uv_out.y < 0.1 || vert_uv_out.y > 0.9) {
        frag_color = vec4(0);
    } else {
        frag_color = texture(scene_tex, vert_uv_out);
    }
    */

    /* Blackband (pixel based)
    if (gl_FragCoord.y < 30 || gl_FragCoord.y > 570) {
        frag_color = vec4(0);
    } else {
        frag_color = texture(scene_tex, vert_uv_out);
    }
    */

    // Blur
    //frag_color = blur();

    // Wave
    //frag_color = wave();

    // Quake
    frag_color = quake();
}