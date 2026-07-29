#version 330 core

layout (location=0) in vec3 pos;
layout (location=1) in vec2 tex;

out vec2 texCoords;

void main()
{
    gl_Position = vec4(pos, 1.0f);
    texCoords = vec2(tex.xy);
}