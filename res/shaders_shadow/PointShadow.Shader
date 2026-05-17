#shader vertex 
#version 420 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoords;

uniform mat4 u_Model;

void main()
{
    gl_Position = u_Model * vec4(a_Position, 1.0);
}

#shader geometry
#version 420 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

uniform mat4 lightSpaceMatrix[6];
out vec4 FragPos;

void main()
{
    for (int face = 0; face < 6; face++)
    {
        gl_Layer = face;
        for (int i = 0; i < 3; i++) {
            FragPos = gl_in[i].gl_Position;
            gl_Position = lightSpaceMatrix[face] * FragPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}

#shader fragment
#version 420 core

in vec4 FragPos;

uniform vec3 lightPos;
uniform float near_plane;
uniform float far_plane;

void main()
{
    float lightDistance = length(FragPos.xyz - lightPos);
    gl_FragDepth = (lightDistance - near_plane) / (far_plane - near_plane);
    //gl_FragDepth = gl_FragCoord.z;
}