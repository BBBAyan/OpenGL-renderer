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
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{ 
    vec3 color = texture(screenTexture, TexCoords).rgb;
    FragColor = vec4(color, 1.0);
    
    float luminescence = dot(color, vec3(0.2126, 0.7152, 0.0722));
    if (luminescence > 0.9) {
        BrightColor = vec4(color, 1.0);
    }else {
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}