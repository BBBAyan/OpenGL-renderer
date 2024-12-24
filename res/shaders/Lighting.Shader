#shader vertex 
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec2 a_TexCoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

out vec2 TexCoords;

void main()
{
    gl_Position = u_Proj * u_View * u_Model * vec4(a_Position, 1.0);
    TexCoords = a_TexCoords;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 fragColor;

in vec2 TexCoords;

uniform sampler2D u_Texture;

void main()
{
    fragColor = texture(u_Texture, TexCoords);
    //fragColor = vec4(0.9, 0.9, 0.9, 1.0);
}