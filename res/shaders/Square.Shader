#shader vertex 
#version 420 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

layout(std140, binding = 0) uniform Matrices
{
    mat4 u_Proj; //16*4
    mat4 u_View; //16*4
};
uniform mat4 u_Model;

void main()
{
    gl_Position = u_Proj * u_View * u_Model * position;
}

#shader fragment
#version 420 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{
    //color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    color = u_Color;
}