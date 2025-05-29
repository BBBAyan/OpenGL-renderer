#pragma once

#include "Renderer.h"
#include "Mesh.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"


class Model {
public:
	std::vector<meshTexture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;

	Model(std::string const &path);
	void Draw(Shader& shader);
private:
	void loadModel(std::string const &path);
	void processNode(aiNode* Node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<meshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};