#pragma once

//#include <string>
//#include <vector>
#include "Renderer.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct meshTexture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<meshTexture> textures;
	unsigned int VAO;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<meshTexture> textures);
	void Draw(Shader& shader);
private:
	unsigned int VBO, EBO;
	void setupMesh();
};