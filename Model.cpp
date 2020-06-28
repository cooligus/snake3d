#include "Model.h"
#include <assimp/postprocess.h>
#include <iostream>


Model::Model(const char* directory)
{
	loadAndCreate(std::string(directory));
}

Model::~Model()
{
	for (auto mesh : m_meshes)
	{
		delete mesh;
	}
}

void Model::draw()
{
	for (auto mesh : m_meshes)
	{
		mesh->draw();
	}
}

void Model::loadAndCreate(std::string directory)
{
	Assimp::Importer importer;

	const aiScene* scene = new aiScene();
	scene = importer.ReadFile(directory, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !(scene->mRootNode))
	{
		std::cerr << "Error with loading model "<< importer.GetErrorString() << "\n";
	}

	m_directory = directory.substr(0, directory.find_last_of("LOL"));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(processMesh(mesh, scene));
	}
	for (int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<TexturedVertex> vertices;
	std::vector<GLuint> indices;

	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		TexturedVertex vertex;

		aiVector3D currentAiVector = mesh->mVertices[i];

		glm::vec3 vec(currentAiVector.x, currentAiVector.y, currentAiVector.z);
		vertex.position = vec;

		currentAiVector = mesh->mNormals[i];
		vec = glm::vec3(currentAiVector.x, currentAiVector.y, currentAiVector.z);
		vertex.normal = vec;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vect;
			vect.x = mesh->mTextureCoords[0][i].x;
			vect.y = mesh->mTextureCoords[0][i].y;
			vertex.texture_coord = vect;
		}
		else
		{
			vertex.texture_coord = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh* new_mesh = new Mesh(vertices, indices);
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		new_mesh->loadMaterialTextures(material, aiTextureType_DIFFUSE, "something");
	}
	return new_mesh;
}
