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

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

layout(location = 0) out vec4 fragColor;

in vec3 Normal;
in vec3 fragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform sampler2D u_Texture;
uniform Light light;
uniform bool blinn;

void main()
{
    vec3 ambient = light.ambient * 0.5;

    vec3 norm = normalize(Normal);
    //vec3 lightDir = normalize(-light.direction);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_Texture, TexCoords));

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec;
    if (blinn) {
        spec = pow(max(dot(halfwayDir, norm), 0.0), light.shininess);
    } else {
        spec = pow(max(dot(reflectDir, viewDir), 0.0), light.shininess);
    }
    vec3 specular = light.specular * spec * 0.5;

    fragColor = 1.0 * vec4(ambient + diffuse + specular, 1.0);
}