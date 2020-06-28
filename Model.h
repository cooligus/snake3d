#pragma once

#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class Model
{
public:
	Model(const char* directory);
	~Model();
	
	void draw();
	

private:
	std::vector<Mesh*>m_meshes;
	std::string m_directory;

	void loadAndCreate(std::string directory);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
};

