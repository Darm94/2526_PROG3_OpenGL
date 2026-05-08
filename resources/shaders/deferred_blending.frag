#version 460 core

in vec2 vert_uv_out;

layout (binding = 0) uniform sampler2D diffuse_tex;
layout (binding = 1) uniform sampler2D normal_tex;
layout (binding = 2) uniform sampler2D position_tex;

out vec4 frag_color;

//vec3 point_light_pos = vec3(4, 0, 0);

const int NUM_LIGHTS = 3;
uniform vec3 point_light_poses[NUM_LIGHTS];

vec3 phong(vec3 point_light_pos) {
    vec3 pixel_color = texture(diffuse_tex, vert_uv_out).rgb;
    vec3 vert_world_norm_out = texture(normal_tex, vert_uv_out).xyz;
    vec3 vert_world_pos_out = texture(position_tex, vert_uv_out).xyz;
    
    vec3 world_norm = normalize(vert_world_norm_out);

    // Ambient
    float ambient_factor = 0.2f;
    vec3 ambient = pixel_color * ambient_factor;

    // Diffuse
    vec3 light_dir = normalize(point_light_pos - vert_world_pos_out);
    float lambert = max(dot(world_norm, light_dir), 0.f);
    vec3 diffuse = pixel_color * lambert;

    vec3 phong = vec3(0.f);
    phong += ambient;
    phong += diffuse;
    return phong;
}

void main() 
{
    //frag_color = texture(diffuse_tex, vert_uv_out);

    frag_color = vec4(0);
    for(int i = 0; i < NUM_LIGHTS; ++i) 
    {
        frag_color += vec4(phong(point_light_poses[i]), 1.f);
    }
}
