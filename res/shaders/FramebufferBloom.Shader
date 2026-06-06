#shader vertex
#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    TexCoords = aTexCoords;
}  
#shader fragment
#version 330 core

layout (location = 0) out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform bool horizontal;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{ 
    vec2 texSize = 1.0 / textureSize(screenTexture, 0);
    vec3 res = texture(screenTexture, TexCoords).rgb * weight[0];
    if (horizontal) {
        for (int i = 1; i < 5; i++) {
            res += texture(screenTexture, TexCoords + texSize * vec2(i, 0.0)).rgb * weight[i];
            res += texture(screenTexture, TexCoords - texSize * vec2(i, 0.0)).rgb * weight[i];
        }
    }else {
        for (int i = 1; i < 5; i++) {
            res += texture(screenTexture, TexCoords + texSize * vec2(0.0, i)).rgb * weight[i];
            res += texture(screenTexture, TexCoords - texSize * vec2(0.0, i)).rgb * weight[i];
        }
    }
    FragColor = vec4(res, 1.0);
}