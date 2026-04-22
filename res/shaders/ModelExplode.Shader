#shader vertex 
#version 420 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec2 TexCoords;
    vec3 Normal;
    vec3 fragPos;
} vs_out;

layout(std140, binding = 0) uniform Matrices
{
    mat4 u_Proj; //16*4
    mat4 u_View; //16*4
};
uniform mat4 u_Model;

void main()
{
    vs_out.TexCoords = aTexCoords;
    vs_out.Normal = mat3(transpose(inverse(u_Model))) * aNormal;
    vs_out.fragPos = vec3(u_Model * vec4(aPos, 1.0));
    gl_Position = u_Proj * u_View * u_Model * vec4(aPos, 1.0);
}

#shader geometry
#version 420 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 TexCoords;
    vec3 Normal;
    vec3 fragPos;
} gs_in[];

out GS_OUT {
    vec2 TexCoords;
    vec3 Normal;
    vec3 fragPos;
} gs_out;

uniform float time;

vec3 GetNormal() {
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

vec4 explode(vec4 pos, vec3 normal) {
    float magnitude = 2.0;
    vec3 dir = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
    return pos + vec4(dir, 0.0);
}

void main()
{
    vec3 normal = GetNormal();
    gl_Position = explode(gl_in[0].gl_Position, normal);
    gs_out.TexCoords = gs_in[0].TexCoords;
    gs_out.Normal = gs_in[0].Normal;
    gs_out.fragPos = gs_in[0].fragPos;
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position, normal);
    gs_out.TexCoords = gs_in[1].TexCoords;
    gs_out.Normal = gs_in[1].Normal;
    gs_out.fragPos = gs_in[1].fragPos;
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position, normal);
    gs_out.TexCoords = gs_in[2].TexCoords;
    gs_out.Normal = gs_in[2].Normal;
    gs_out.fragPos = gs_in[2].fragPos;
    EmitVertex();
    EndPrimitive();
}

#shader fragment
#version 420 core

out vec4 FragColor;

in GS_OUT {
    vec2 TexCoords;
    vec3 Normal;
    vec3 fragPos;
} fs_in;

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
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

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir);

uniform DirLight dirLight;
uniform PointLight pointLight;
uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
    vec3 normal = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.fragPos);

    vec3 output = vec3(0.0);
    output += CalcDirLight(dirLight, normal, viewDir);
    output += CalcPointLight(pointLight, normal, viewDir);
    vec4 textureColour = texture(texture_diffuse1, fs_in.TexCoords);
    if (textureColour.a < 0.5) discard;

    FragColor = vec4(output, 1.0);
    float near = 0.1;
    float far = 100.0;
    float ndc = gl_FragCoord.z * 2.0 - 1.0;
    float linearDepth = (2.0 * near * far) * (far + near - ndc * (far - near));
    //FragColor = vec4(vec3(1 - linearDepth / far), 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, fs_in.TexCoords));

    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, fs_in.TexCoords));

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 51.2);
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, fs_in.TexCoords));

    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fs_in.fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = vec3(1.0, 1.0, 1.0) * light.diffuse * diff * vec3(texture(texture_diffuse1, fs_in.TexCoords));

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 51.2);
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, fs_in.TexCoords));

    float distance = length(light.position - fs_in.fragPos);
    float attentuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    return attentuation * (diffuse + specular);
}
