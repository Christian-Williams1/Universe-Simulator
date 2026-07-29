#version 330 core

layout (location=0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 col;
uniform vec3 worldPos;
//uniform mat3 transposeInverseModel;

out vec3 normal;
out vec3 color;
out vec3 lightDir;

void main()
{
    gl_Position = projection*view*model*vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    color = col;

    //lightDir = normalize(-vec3(model*vec4(aPos, 1.0f)));
    lightDir = normalize(-worldPos);

    normal = normalize(aPos);
}