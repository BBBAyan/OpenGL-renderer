#shader vertex 
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;
layout(location = 2) in vec3 a_Normal;

uniform mat4 u_MVP;

out vec3 vertexColor;
out vec3 Normal;
out vec3 fragPos;

void main()
{
    gl_Position = u_MVP * vec4(a_Position, 1.0);
    vertexColor = a_Color;
    Normal = a_Normal;
    fragPos = a_Position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 fragColor;

in vec3 vertexColor;
in vec3 Normal;
in vec3 fragPos;

uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    float ambientStrength = 0.0;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = (ambient + diffuse) * vertexColor;
    fragColor = vec4(result, 1.0);
}