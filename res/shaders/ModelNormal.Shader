#shader vertex 
#version 420 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec3 Normal;
} vs_out;

uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
    mat3 NormalMatrix = mat3(transpose(inverse(u_View * u_Model)));
    vs_out.Normal = normalize(vec3(vec4(NormalMatrix * aNormal, 0.0)));
    gl_Position = u_View * u_Model * vec4(aPos, 1.0);
}

#shader geometry
#version 420 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 Normal;
} gs_in[];

uniform mat4 u_Proj;

const float Magnitude = 0.1;

void GenerateLine(int index)
{
    gl_Position = u_Proj * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = u_Proj * (gl_in[index].gl_Position + vec4(gs_in[index].Normal, 0.0) * Magnitude);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0);
    GenerateLine(1);
    GenerateLine(2);
}

#shader fragment
#version 420 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}