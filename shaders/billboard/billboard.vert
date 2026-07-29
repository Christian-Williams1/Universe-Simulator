#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec2 atexCoord;

out vec2 texCoord;

uniform mat4 mv;

void main()
{
    gl_Position = mv*vec4(aPos, 1.0f);
    texCoord = atexCoord;
}