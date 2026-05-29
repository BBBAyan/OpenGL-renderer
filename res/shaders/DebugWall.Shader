#shader vertex 
#version 420 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoords;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

layout(std140, binding = 0) uniform Matrices
{
    mat4 u_Proj; //16*4
    mat4 u_View; //16*4
};
uniform mat4 u_Model;
uniform mat4 lightSpaceMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightPos;
uniform vec3 lightDir;
uniform vec3 viewPos;

out VS_OUT {
    vec2 TexCoords;
    vec3 FragPos;
    vec3 lightPos;
    vec3 lightDir;
    vec3 TangentFragPos;
    vec3 TangentViewPos;
    vec3 TangentLightPos;
    vec3 TangentDirLight;
    vec4 fragPosLightSpace;
    //
    vec3 T, B, N;
} vs_out;

void main()
{
    gl_Position = u_Proj * u_View * u_Model * vec4(a_Position, 1.0);
    vec3 T = normalize(normalMatrix * a_Tangent);
    vec3 B = normalize(normalMatrix * a_Bitangent);
    vec3 N = normalize(normalMatrix * a_Normal);
    vs_out.T = T;vs_out.B = B;vs_out.N = N;
    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.lightPos = lightPos;
    vs_out.lightDir = lightDir;
    vs_out.FragPos = vec3(u_Model * vec4(a_Position, 1.0));
    vs_out.TangentFragPos = TBN * vs_out.FragPos;
    vs_out.TangentViewPos = TBN * viewPos;
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentDirLight = TBN * lightDir;
    vs_out.TexCoords = a_TexCoords;
    vs_out.fragPosLightSpace = lightSpaceMatrix * u_Model * vec4(a_Position, 1.0);
}

#shader geometry
#version 420 core

layout(triangles) in;
layout(line_strip, max_vertices = 24) out;

layout(std140, binding = 0) uniform Matrices
{
    mat4 u_Proj;
    mat4 u_View;
};

in VS_OUT {
    vec2 TexCoords;
    vec3 FragPos;
    vec3 lightPos;
    vec3 lightDir;
    vec3 TangentFragPos;
    vec3 TangentViewPos;
    vec3 TangentLightPos;
    vec3 TangentDirLight;
    vec4 fragPosLightSpace;
    vec3 T;
    vec3 B;
    vec3 N;
} gs_in[];

out vec3 g_Color;

const float LINE_LENGTH = 2.0;

void EmitLine(vec3 origin, vec3 dir, vec3 color)
{
    g_Color = color;
    gl_Position = u_Proj * u_View * vec4(origin, 1.0);
    EmitVertex();
    gl_Position = u_Proj * u_View * vec4(origin + dir * LINE_LENGTH, 1.0);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    for (int i = 0; i < 3; i++)
    {
        vec3 pos = gs_in[i].FragPos;
        EmitLine(pos, gs_in[i].T, vec3(1.0, 0.0, 0.0)); // red   = Tangent
        EmitLine(pos, gs_in[i].B, vec3(0.0, 1.0, 0.0)); // green = Bitangent
        EmitLine(pos, gs_in[i].N, vec3(0.0, 0.0, 1.0)); // blue  = Normal
    }
    // 3 vertices * 3 axes * 2 vertices per line = 18 vertices, fits in max_vertices = 24
}

#shader fragment
#version 420 core

in vec3 g_Color;
layout(location = 0) out vec4 fragColor;

void main()
{
    fragColor = vec4(g_Color, 1.0);
}