#shader vertex
#version 420 core

layout (location = 0) in vec3 aPos;

out vec3 vNormal

int main() {

}

#shader geometry
#version 420 core

float point[] = {
    -0.5f,  0.5f, //top left
     0.5f,  0.5f, //top right
    -0.5f, -0.5f, //bottom left
     0.5f,  0.5f  //bottom right
}

layout (triangles) in;
layout (points, max_vertices = 2) out;

in vec3 vNormal[];
in vec3 vfragPos[];
in vec2 vTexCoords[];

out vec3 Normal;
out vec3 fragPos;
out vec2 TexCoords;
  
void main() {    
    for (int i = 0; i < points.size(); i++) {
        gl_Position = gl_in[0].gl_Position + points[i];
        Normal = vNormal[0];
        fragPos = vfragPos[0];
        TexCoords = vTexCoords[0];
        EmitVertex();
    }
    
    EndPrimitive();
}    
#shader fragment
#version 420 core

in vec3 Normal;
in vec3 fragPos;
in vec2 TexCoords;

int main() {

}