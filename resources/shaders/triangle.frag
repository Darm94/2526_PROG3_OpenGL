#version 460 core

in vec3 vertColor;
out vec4 frag_color;

void main() 
{
    frag_color = vec4(vertColor, 1.0);
}
