#shader vertex 
#version 420 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoords;

layout(std140, binding = 0) uniform Matrices
{
    mat4 u_Proj; //16*4
    mat4 u_View; //16*4
};
uniform mat4 u_Model;

out vec3 Normal;
out vec3 fragPos;
out vec2 TexCoords;

void main()
{
    gl_Position = u_Proj * u_View * u_Model * vec4(a_Position, 1.0);
    Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
    fragPos = vec3(u_Model * vec4(a_Position, 1.0));
    TexCoords = a_TexCoords;
}

#shader fragment
#version 420 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
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

layout(location = 0) out vec4 fragColor;

in vec3 Normal;
in vec3 fragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform PointLight pointLight;
uniform vec3 pointLightColor;

void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 output = vec3(0.0);

    output += CalcPointLight(pointLight, normal, viewDir);
    fragColor = vec4(output, 1.0);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir) {
    vec3 ambient = 2 * light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = pointLightColor * light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    float distance = length(light.position - fragPos);
    float attentuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    return attentuation * (ambient + diffuse + specular);
}