#shader vertex
#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
    TexCoords = aTexCoords;
}  
#shader fragment
#version 330 core

out vec4 FragColor;
  
in vec2 TexCoords;

const float offset = 1.0/300.0;
uniform sampler2D screenTexture;

/*Regular
void main()
{ 
    FragColor = texture(screenTexture, TexCoords);
}
*/
/*Inversion
void main()
{
    FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
}
*/
/*Gray Scale
void main()
{
    FragColor = texture(screenTexture, TexCoords);
    float avg = 0.2 * FragColor.r + 0.7 * FragColor.g + 0.1 * FragColor.b;
    FragColor = vec4(avg, avg, avg, 1.0);
}
*/
/*Kernel
void main()
{
    vec2 offsets[9] = vec2[9](
        vec2(-offset, -offset),
        vec2(-offset, 0.0),
        vec2(-offset, offset),
        vec2(0.0, -offset),
        vec2(0.0, 0.0),
        vec2(0.0, offset),
        vec2(offset, -offset),
        vec2(offset, 0.0),
        vec2(offset, offset)
    );
    float kernel[9] = float[9](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );

    vec3 sum = vec3(0.0);
    for (int i = 0; i < 9; i++){
        sum += vec3(texture(screenTexture, TexCoords.st + offsets[i])) * kernel[i];
    }
    FragColor = vec4(sum, 1.0);
}
*/
/*Blur
void main()
{
    vec2 offsets[9] = vec2[9](
        vec2(-offset, -offset),
        vec2(-offset, 0.0),
        vec2(-offset, offset),
        vec2(0.0, -offset),
        vec2(0.0, 0.0),
        vec2(0.0, offset),
        vec2(offset, -offset),
        vec2(offset, 0.0),
        vec2(offset, offset)
    );
    float kernel[9] = float[9](
        1, 2, 1,
        2, 4, 2,
        1, 2, 1
    );

    vec3 sum = vec3(0.0);
    for (int i = 0; i < 9; i++){
        sum += vec3(texture(screenTexture, TexCoords.st + offsets[i])) * (kernel[i] / 16);
    }
    FragColor = vec4(sum, 1.0);
}
*/
/*Edge detection
void main()
{
    vec2 offsets[9] = vec2[9](
        vec2(-offset, -offset),
        vec2(-offset, 0.0),
        vec2(-offset, offset),
        vec2(0.0, -offset),
        vec2(0.0, 0.0),
        vec2(0.0, offset),
        vec2(offset, -offset),
        vec2(offset, 0.0),
        vec2(offset, offset)
    );
    float kernel[9] = float[9](
        1,  1, 1,
        1, -8, 1,
        1,  1, 1
    );

    vec3 sum = vec3(0.0);
    for (int i = 0; i < 9; i++){
        sum += vec3(texture(screenTexture, TexCoords.st + offsets[i])) * kernel[i];
    }
    FragColor = vec4(sum, 1.0);
}
*/
/*Testing Kernels
*/
void main()
{
    vec2 offsets[9] = vec2[9](
        vec2(-offset, -offset),
        vec2(-offset, 0.0),
        vec2(-offset, offset),
        vec2(0.0, -offset),
        vec2(0.0, 0.0),
        vec2(0.0, offset),
        vec2(offset, -offset),
        vec2(offset, 0.0),
        vec2(offset, offset)
    );
    float kernel[9] = float[9](
        -1, -2, -1,
        0, 0, 0,
        1, 2, 1
    );

    vec3 sum = vec3(0.0);
    for (int i = 0; i < 9; i++){
        sum += vec3(texture(screenTexture, TexCoords.st + offsets[i])) * (kernel[i] / 16);
    }
    FragColor = vec4(sum, 1.0);
}