#pragma once
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <GLM/glm.hpp>
#include <vector>
#include "Debug.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

template<typename T>
void glBufferDataFromVector(GLenum target, const std::vector<T>& buffer, GLenum type)
{
	glCheck(glBufferData(target, sizeof(T) * buffer.size(), &buffer[0], type));
}

struct TexturedVertex
{
	glm::vec3 position;
	glm::vec2 texture_coord;
	glm::vec3 normal;
};

class Mesh
{
public:
	Mesh(std::vector<TexturedVertex>& m_vertex_buffer, std::vector<GLuint>& m_element_buffer);
	~Mesh();

	void draw(ShaderProgram* shader)const;
	void draw()const;
	void bind()const;
	void unBind()const;

	void loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string type_name);

	GLuint getVAO();
	GLuint getVBO();
	GLuint getEBO();

private:
	void init();

	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;

	std::vector<Texture*>m_textures;
	std::vector<TexturedVertex> m_vertex_buffer;
	std::vector<GLuint> m_element_buffer;
};

