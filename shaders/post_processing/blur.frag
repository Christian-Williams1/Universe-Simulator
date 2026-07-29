#version 330 core

out vec4 fragCol;

in vec2 texCoords;
uniform sampler2D fboTex;

uniform bool horizontal;

uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

// keep this code without the offsets for vid later
/*
vec4 blur(sampler2D sourceTexture, vec2 blurDirection, vec2 pixelCoord)
{
    vec4 result = vec4(0.0);
    vec2 size = textureSize(sourceTexture, 0);
    for (int i = 0; i < SAMPLE_COUNT; ++i)
    {
        vec2 offset = blurDirection * OFFSETS[i] / size;
        float weight = WEIGHTS[i];
        result += texture(sourceTexture, pixelCoord + offset) * weight;
    }
    return result;
}
*/
void main()
{
    
    vec2 tex_offset = 1.0 / textureSize(fboTex, 0); // gets size of single texel
    vec3 result = texture(fboTex, texCoords).rgb * weight[0]; // current fragment's contribution
    if(horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(fboTex, texCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(fboTex, texCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(fboTex, texCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(fboTex, texCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }
    
    fragCol = vec4(result, 1.0);
}