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
}

#shader fragment
#version 420 core

out vec4 FragColor;

void main() {
	FragColor = vec4(0.4, 0.1, 0.4, 1.0);
}