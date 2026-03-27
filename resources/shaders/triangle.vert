#version 460 core
//for macos 410 core

layout (location = 0) in vec3 vert_pos;
layout(location = 1) in vec3 aColor;

out vec3 vertColor;

void main() 
{
    gl_Position = vec4(vert_pos, 1.f);
    vertColor = aColor;
}
