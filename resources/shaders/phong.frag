#version 460 core

in vec2 vert_uv_out;
in vec3 vert_world_norm_out;
in vec3 vert_world_pos_out;

layout (binding = 0) uniform sampler2D trup_tex;

out vec4 frag_color;

void main() 
{
    vec3 point_light_pos = vec3(4, 0, 0);

    vec4 texel = texture(trup_tex, vert_uv_out);
    vec3 pixel_color = texel.xyz;
    
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
    
    frag_color = vec4(phong, 1.f);
}