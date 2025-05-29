#shader vertex 
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 fragPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
    TexCoords = aTexCoords;
    Normal = aNormal;//mat3(transpose(inverse(u_Model))) * aNormal;
    gl_Position = u_Proj * u_View * u_Model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 fragPos;

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir);

uniform PointLight pointLight;
uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 output = vec3(0.0);
    output += CalcPointLight(pointLight, normal, viewDir);
    FragColor = vec4(output, 1.0);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir) {
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));

    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = vec3(1.0, 1.0, 1.0) * light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 51.2);
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));

    float distance = length(light.position - fragPos);
    float attentuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    return attentuation * (ambient + diffuse + specular);
}
