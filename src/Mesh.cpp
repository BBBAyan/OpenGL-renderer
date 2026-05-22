#include "Mesh.h"		

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO_TANGENTS);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindBuffer(GL_ARRAY_BUFFER, VBO_TANGENTS);
	glBufferData(GL_ARRAY_BUFFER, normalData.size() * sizeof(NormalData), &normalData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(NormalData), (void*)0);

	glBindVertexArray(0);
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<meshTexture> textures, std::vector<NormalData> normalData)
{
	this->vertices = vertices;
	this->normalData = normalData;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
}

void Mesh::Draw(Shader& shader)
{
	unsigned int diffuseN = 1;
	unsigned int specularN = 1;
	unsigned int normalN = 1;
	
	for (unsigned int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		if (textures[i].type == "texture_diffuse")
			number = std::to_string(diffuseN++);
		else if (textures[i].type == "texture_specular")
			number = std::to_string(specularN++);
		else if (textures[i].type == "texture_normal")
			number = std::to_string(normalN++);

		shader.SetUniform1i(/*"material." + */textures[i].type + number, i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}
