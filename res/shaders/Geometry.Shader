#shader vertex
#version 420 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

out vec3 color;
//out vec3 vNormal;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    color = aColor;

}

#shader geometry
#version 420 core

layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in vec3 color[];

out vec3 gcolor;

vec2 house[] = vec2[](
    vec2(-0.1, -0.1), //left bottom
    vec2(0.1, -0.1),  //right bottom
    vec2(-0.1,  0.1), //left top
    vec2(0.1,  0.1),  //right top
    vec2(0.0,  0.2)   //top
);

void main() {
    gcolor = color[0];
    vec4 center = gl_in[0].gl_Position;
    for (int i = 0; i < 5; i++) {
        gl_Position = center + vec4(house[i], 0.0, 0.0);
        EmitVertex();
    }
    EndPrimitive();
}

#shader fragment
#version 420 core

in vec3 gcolor;
out vec4 FragColor;

//in vec3 Normal;
//in vec3 fragPos;
//in vec2 TexCoords;

void main() {
    //if (gcolor = )
    //FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    FragColor = vec4(gcolor, 1.0);
}