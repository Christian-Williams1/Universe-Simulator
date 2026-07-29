#version 330 core

layout (location=0) out vec4 fragCol;
layout (location=1) out vec4 brightColor;

in vec3 color;

void main()
{
    fragCol = vec4(color*3.0f, 1.0f);
    brightColor = vec4(color*10.0f, 1.0f);
}