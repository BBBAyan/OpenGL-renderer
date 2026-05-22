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
out VS_OUT {
    vec2 TexCoords;
    vec3 FragPos;
    vec3 lightPos;
    vec3 TangentFragPos;
    vec3 TangentViewPos;
    vec3 TangentLightPos;
    vec3 TangentLightDir;
    vec4 fragPosLightSpace;
} vs_out;

uniform mat4 u_Model;
uniform mat4 lightSpaceMatrix;
uniform vec3 lightPos;
uniform vec3 lightDir;
uniform vec3 viewPos;

void main()
{
    gl_Position = u_Proj * u_View * u_Model * vec4(a_Position, 1.0);
    mat3 normalMatrix = transpose(inverse(mat3(u_Model)));
    vec3 T = normalize(normalMatrix * a_Tangent);
    vec3 B = normalize(normalMatrix * a_Bitangent);
    vec3 N = normalize(normalMatrix * a_Normal);
    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.lightPos = lightPos;
    vs_out.FragPos = vec3(u_Model * vec4(a_Position, 1.0));
    vs_out.TangentFragPos = TBN * vs_out.FragPos;
    vs_out.TangentViewPos = TBN * viewPos;
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentLightDir = TBN * lightDir;
    vs_out.TexCoords = a_TexCoords;
    vs_out.fragPosLightSpace = lightSpaceMatrix * u_Model * vec4(a_Position, 1.0);
}

#shader fragment
#version 420 core

struct Material {
    sampler2D diffuse;
    sampler2D normal;
    float shininess;
};
struct DirLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
};
struct PointLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

layout(location = 0) out vec4 fragColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float repeatFactor);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, float repeatFactor);
float ShadowDirCalculation(vec4 fragPosLightSpace, vec3 normal);
float ShadowPointCalculation();

in VS_OUT {
    vec2 TexCoords;
    vec3 FragPos;
    vec3 lightPos;
    vec3 TangentFragPos;
    vec3 TangentViewPos;
    vec3 TangentLightPos;
    vec3 TangentLightDir;
    vec4 fragPosLightSpace;
} fs_in;

uniform sampler2D shadowMap;
uniform samplerCube pointDepthMap;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform bool blinn;
uniform float far_plane;
uniform float near_plane;
uniform float repeatFactor;

void main()
{
    vec2 Coords = fs_in.TexCoords * repeatFactor;
    vec3 normal = texture(material.normal, Coords).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);

    vec3 ambientDir = dirLight.ambient * vec3(texture(material.diffuse, Coords));
    vec3 ambientPoint = pointLight.ambient * vec3(texture(material.diffuse, Coords));

    float shadowDir = ShadowDirCalculation(fs_in.fragPosLightSpace, normal);
    float shadowPoint = ShadowPointCalculation();
    vec3 lightDir = (1.0 - shadowDir) * CalcDirLight(dirLight, normal, viewDir, repeatFactor) * dirLight.intensity;
    vec3 lightPoint = (1.0 - shadowPoint) * CalcPointLight(pointLight, normal, viewDir, repeatFactor);
    vec3 output = lightDir + lightPoint + ambientDir + ambientPoint;
    fragColor = vec4(output, 1.0);
    //fragColor = vec4(fs_in.N * 0.5 + 0.5, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float repeatFactor) {
    vec3 lightDir = normalize(-fs_in.TangentLightDir);
    float diff = max(dot(normal, lightDir), 0.0);
    vec2 Coords = fs_in.TexCoords * repeatFactor;
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, Coords));

    vec3 specular = vec3(0.0);
    if (diff > 0.0) {
        vec3 reflectDir = reflect(-lightDir, normal);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec;
        if (blinn) {
            spec = pow(max(dot(halfwayDir, normal), 0.0), material.shininess * 4);
        } else {
            spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess * 4);
        }
        specular = light.specular * spec;
    }

    return diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, float repeatFactor) {
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec2 Coords = fs_in.TexCoords * repeatFactor;
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, Coords));

    vec3 specular = vec3(0.0);
    if (diff > 0.0) {
        vec3 reflectDir = reflect(-lightDir, normal);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec;
        if (blinn) {
            spec = pow(max(dot(halfwayDir, normal), 0.0), material.shininess * 4);
        } else {
            spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess * 4);
        }
        specular = light.specular * spec;
    }

    float distance = length(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float attentuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    return attentuation * (diffuse + specular);
}

float ShadowDirCalculation(vec4 fragPosLightSpace, vec3 normal) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    if (currentDepth > 1.0) return 0.0;
    vec3 dirLight = normalize(-fs_in.TangentLightDir);
    float bias = max(0.01 * (1.0 - dot(normal, dirLight)), 0.001);
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for (int i = -1; i <= 1; i++){
        for (int j = -1; j <= 1; j++) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(i, j) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    return shadow;
}

float ShadowPointCalculation() {
    vec3 fragToLight = fs_in.FragPos - fs_in.lightPos;
    //vec3 fragToLight = fs_in.TangentFragPos - fs_in.TangentLightPos;
    float closestDepth = texture(pointDepthMap, fragToLight).r;
    closestDepth = closestDepth * (far_plane - near_plane) + near_plane;
    float curDepth = length(fragToLight);
    float bias = 0.05;
    //float texelSize = 0.1;
    float texelSize = (1.0 + (curDepth / far_plane)) / 60.0;
    float shadow = 0.0;
    int samples = 1;
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