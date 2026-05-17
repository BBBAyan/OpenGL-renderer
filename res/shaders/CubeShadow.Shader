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
uniform mat4 lightSpaceMatrix;

out VS_OUT {
    vec2 TexCoords;
    vec3 Normal;
    vec3 fragPos;
    vec4 fragPosLightSpace;
} vs_out;

void main()
{
    gl_Position = u_Proj * u_View * u_Model * vec4(a_Position, 1.0);
    vs_out.Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
    vs_out.fragPos = vec3(u_Model * vec4(a_Position, 1.0));
    vs_out.TexCoords = a_TexCoords;
    vs_out.fragPosLightSpace = lightSpaceMatrix * vec4(vs_out.fragPos, 1.0);
}

#shader fragment
#version 420 core
struct Material {
    sampler2D diffuse;
    float shininess;
};
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
};
struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir);
float ShadowDirCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);
float ShadowPointCalculation(vec3 lightPos);

layout(location = 0) out vec4 fragColor;

in VS_OUT {
    vec2 TexCoords;
    vec3 Normal;
    vec3 fragPos;
    vec4 fragPosLightSpace;
} fs_in;

uniform sampler2D shadowMap;
uniform samplerCube pointDepthMap;
uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform float far_plane;
uniform float near_plane;

void main()
{
    vec3 normal = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.fragPos);

    vec3 ambientDir = dirLight.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 ambientPoint = pointLight.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));

    float shadowDir = ShadowDirCalculation(fs_in.fragPosLightSpace, normal, dirLight.direction);
    float shadowPoint = ShadowPointCalculation(pointLight.position);
    vec3 lightDir = (1.0 - shadowDir) * CalcDirLight(dirLight, normal, viewDir) * dirLight.intensity;
    vec3 lightPoint = (1.0 - shadowPoint) * CalcPointLight(pointLight, normal, viewDir);
    vec3 output = lightDir + lightPoint + ambientDir + ambientPoint;
    fragColor = vec4(output, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));

    vec3 specular = vec3(0.0);
    if (diff > 0.0) {
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        specular = light.specular * spec;
    }

    return diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fs_in.fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));

    vec3 specular = vec3(0.0);
    if (diff > 0.0) {
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        specular = light.specular * spec;
    }

    float distance = length(light.position - fs_in.fragPos);
    float attentuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    return attentuation * (diffuse + specular);
}

float ShadowDirCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    if (currentDepth > 1.0) return 0.0;
    vec3 dirLight = normalize(-lightDir);
    float bias = max(0.005 * (1.0 - dot(normal, dirLight)), 0.0005);
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    //vec2 texelSize = vec2(0.0, 0.0);
    for (int i = -1; i <= 1; i++){
        for (int j = -1; j <= 1; j++) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(i, j) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    return shadow;
}

float ShadowPointCalculation(vec3 lightPos) {
    vec3 fragToLight = fs_in.fragPos - lightPos;
    float closestDepth = texture(pointDepthMap, fragToLight).r;
    closestDepth = closestDepth * (far_plane - near_plane) + near_plane;
    float curDepth = length(fragToLight);
    float bias = 0.05;
    //float texelSize = 0.1;
    float texelSize = (1.0 + (curDepth / far_plane)) / 25.0;
    float shadow = 0.0;
    int samples = 2;
    for (int i = -samples; i <= samples; i++) {
        for (int j = -samples; j <= samples; j++) {
            for (int k = -samples; k <= samples; k++) {
                float pcfDepth = texture(pointDepthMap, fragToLight + vec3(i, j, k) * texelSize).r;
                pcfDepth = pcfDepth * (far_plane - near_plane) + near_plane;
                shadow += curDepth - bias > pcfDepth ? 1.0 : 0.0;
            }
        }
    }
    shadow /= (samples * 2 + 1) * (samples * 2 + 1) * (samples * 2 + 1);
    return shadow;
}