#shader vertex 
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

out vec3 Normal;
out vec3 fragPos;

void main()
{
    gl_Position = u_Proj * u_View * u_Model * vec4(a_Position, 1.0);
    Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
    fragPos = a_Position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 fragColor;

in vec3 Normal;
in vec3 fragPos;

uniform mat4 u_Model;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightPosObjectSpace = vec3(inverse(u_Model) * vec4(lightPos, 1.0));
    vec3 lightDir = normalize(lightPosObjectSpace - fragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    float specularStrength = 1.0;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    fragColor = vec4(result, 1.0);
}