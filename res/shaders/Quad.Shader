#shader vertex 
#version 420 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aOffset;

out vec3 color;

uniform mat4 u_Proj;

void main()
{
    vec2 pos = aPos * (gl_InstanceID / 100.0);
    gl_Position = u_Proj * vec4(pos + aOffset, 0.0, 1.0);
    color = aColor;
}

#shader fragment
#version 420 core

layout(location = 0) out vec4 fragColor;

in vec3 color;

void main()
{
   fragColor = vec4(color, 1.0);
}