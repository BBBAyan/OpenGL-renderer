#shader vertex 
#version 420 core

layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec2 a_TexCoords;

out vec2 TexCoords;

layout(std140, binding = 0) uniform Matrices
{
    mat4 u_Proj; //16*4
    mat4 u_View; //16*4
};
uniform mat4 u_Model;

void main()
{
    TexCoords = a_TexCoords;
    gl_Position = u_Proj * u_View * u_Model * vec4(a_Position, 1.0);
    //gl_PointSize = gl_Position.z;
}

#shader fragment
#version 420 core

layout(location = 0) out vec4 fragColor;

in vec2 TexCoords;

uniform vec3 pointLightColor;

void main()
{
    fragColor = vec4(pointLightColor, 1.0);
    //fragColor = vec4(0.9, 0.9, 0.9, 1.0);
}