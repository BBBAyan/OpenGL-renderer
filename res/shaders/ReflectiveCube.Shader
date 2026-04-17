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

layout(location = 0) out vec4 fragColor;

in vec3 Normal;
in vec3 fragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform samplerCube skybox;

/*Reflection
void main()
{
    vec3 I = normalize(fragPos - viewPos);
    vec3 R = reflect(I, normalize(Normal));

    fragColor = vec4(texture(skybox, R).rgb, 1.0);
}
*/
/*Refraction
*/
void main() {
    float ratio = 1.0 / 1.52; // air to glass
    vec3 I = normalize(fragPos - viewPos);
    vec3 R = refract(I, normalize(Normal), ratio);

    fragColor = vec4(texture(skybox, R).rgb, 1.0);
}