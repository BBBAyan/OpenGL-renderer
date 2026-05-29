#pragma once

//#include <string>
//#include <vector>
#include "Renderer.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct NormalData {
	glm::vec3 Tangent;
};

struct meshTexture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh{
public:
	std::vector<Vertex> vertices;
	std::vector<NormalData> normalData;
	std::vector<unsigned int> indices;
	std::vector<meshTexture> textures;
	unsigned int VAO;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<meshTexture> textures, std::vector<NormalData> normaldata);
	void Draw(Shader& shader);
private:
	unsigned int VBO, EBO, VBO_TANGENTS;
	void setupMesh();
};