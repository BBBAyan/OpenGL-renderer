#shader vertex 
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

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
#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outercutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    sampler2D spotlightTex;
};

layout(location = 0) out vec4 fragColor;

in vec3 Normal;
in vec3 fragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir) , 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    float distance = length(light.position - fragPos);
    float attentuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    float theta = dot(lightDir, normalize(-light.direction));
    if (theta < light.cutOff){
        if (theta > light.outercutOff){
            float multiplicator = (theta - light.outercutOff) / (light.cutOff - light.outercutOff);
            fragColor = max(multiplicator * vec4(attentuation * (ambient + diffuse + specular), 1.0), vec4(ambient, 1.0));
        } else
            fragColor = vec4(ambient, 1.0);
    } else
        fragColor = vec4(attentuation * (ambient + diffuse + specular), 1.0);
}