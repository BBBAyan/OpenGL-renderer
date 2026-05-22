#shader vertex 
#version 420 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;

layout(std140, binding = 0) uniform Matrices
{
    mat4 u_Proj; //16*4
    mat4 u_View; //16*4
};
uniform mat4 lightSpaceMatrix;
uniform mat4 u_Model;
uniform vec3 lightPos;
uniform vec3 lightDir;
uniform vec3 viewPos;

out VS_OUT {
    vec2 TexCoords;
    vec3 FragPos;
    vec3 lightPos;
    vec3 TangentFragPos;
    vec3 TangentViewPos;
    vec3 TangentLightDir;
    vec3 TangentLightPos;
    vec4 fragPosLightSpace;
} vs_out;

void main()
{
    gl_Position = u_Proj * u_View * u_Model * vec4(aPos, 1.0);
    ///////////
    //vs_out.TexCoords = vec2(1.0 - aTexCoords.x, aTexCoords.y); 
    vs_out.TexCoords = aTexCoords;
    ///////////
    mat3 normalMatrix = transpose(inverse(mat3(u_Model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 bitangent = cross(N, T);
    vec3 B = normalize(normalMatrix * bitangent);
    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.FragPos = vec3(u_Model * vec4(aPos, 1.0));
    vs_out.lightPos = lightPos;
    vs_out.TangentFragPos = TBN * vs_out.FragPos;
    vs_out.TangentLightDir = TBN * lightDir;
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos = TBN * viewPos;
    vs_out.fragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
}

#shader fragment
#version 420 core

out vec4 FragColor;

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

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir);
float ShadowDirCalculation(vec4 fragPosLightSpace, vec3 normal);
float ShadowPointCalculation();

in VS_OUT {
    vec2 TexCoords;
    vec3 FragPos;
    vec3 lightPos;
    vec3 TangentFragPos;
    vec3 TangentViewPos;
    vec3 TangentLightDir;
    vec3 TangentLightPos;
    vec4 fragPosLightSpace;
} fs_in;

uniform sampler2D shadowMap;
uniform samplerCube pointDepthMap;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform float far_plane;
uniform float near_plane;

void main()
{
    vec3 normal = texture(texture_normal1, fs_in.TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);

    vec3 ambientDir = dirLight.ambient * vec3(texture(texture_diffuse1, fs_in.TexCoords));
    vec3 ambientPoint = pointLight.ambient * vec3(texture(texture_diffuse1, fs_in.TexCoords));

    float shadowDir = ShadowDirCalculation(fs_in.fragPosLightSpace, normal);
    float shadowPoint = ShadowPointCalculation();
    vec3 lightDir = (1.0 - shadowDir) * CalcDirLight(dirLight, normal, viewDir) * dirLight.intensity;
    vec3 lightPoint = (1.0 - shadowPoint) * CalcPointLight(pointLight, normal, viewDir);
    vec3 output = lightDir + lightPoint + ambientDir + ambientPoint;

    FragColor = vec4(output, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-fs_in.TangentLightDir);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, fs_in.TexCoords));

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 51.2);
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, fs_in.TexCoords));

    return diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = vec3(1.0, 1.0, 1.0) * light.diffuse * diff * vec3(texture(texture_diffuse1, fs_in.TexCoords));

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 51.2);
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, fs_in.TexCoords));

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