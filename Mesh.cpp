#include "Mesh.h"
#include <sstream>

Mesh::Mesh(std::vector<TexturedVertex>& m_vertex_buffer, std::vector<GLuint>& m_element_buffer)
	:m_VAO(0), m_VBO(0), m_EBO(0), m_vertex_buffer(m_vertex_buffer), m_element_buffer(m_element_buffer)
{
	init();
}

Mesh::~Mesh()
{
	for (auto tex : m_textures)
	{
		delete tex;
	}

	glCheck(glDeleteBuffers(1, &m_VBO));
	glCheck(glDeleteBuffers(1, &m_EBO));
	glCheck(glDeleteVertexArrays(1, &m_VAO));
}

void Mesh::draw(ShaderProgram* shader) const
{
	unsigned int diffuse1 = 1;
	unsigned int specular1 = 1;
	for (int i = 0; i < this->m_textures.size(); i++)
	{
		glCheck(glActiveTexture(GL_TEXTURE0 + i));

		std::stringstream ss;
		std::string number;
		std::string name = this->m_textures[i]->getType();
		if (name == TEXTURE_TYPE_DIFFUSE)
			ss << diffuse1++;
		else if (name == TEXTURE_TYPE_SPECULAR)
			ss << specular1++;
		number = ss.str();

		shader->setUniform1f((name + number).c_str(), i);
		glCheck(glBindTexture(GL_TEXTURE_2D, this->m_textures[i]->getTextureID()));
	}

	bind();
	glCheck(glDrawElements(GL_TRIANGLES, m_vertex_buffer.size() , GL_UNSIGNED_INT, nullptr));
	unBind();
}

void Mesh::draw() const
{
	bind();
	glCheck(glDrawElements(GL_TRIANGLES, m_vertex_buffer.size(), GL_UNSIGNED_INT, nullptr));
	unBind();
}

void Mesh::bind() const
{
	glCheck(glBindVertexArray(m_VAO));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO));
}

void Mesh::unBind() const
{
	glCheck(glBindVertexArray(0));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

GLuint Mesh::getVAO()
{
	return m_VAO;
}

GLuint Mesh::getVBO()
{
	return m_VBO;
}

GLuint Mesh::getEBO()
{
	return m_EBO;
}

void Mesh::init()
{
	glCheck(glGenVertexArrays(1, &m_VAO));
	glCheck(glGenBuffers(1, &m_VBO));
	glCheck(glGenBuffers(1, &m_EBO));

	glCheck(glBindVertexArray(m_VAO));

	//setting up Vertex buffer object
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	glCheck(glBufferDataFromVector(GL_ARRAY_BUFFER, m_vertex_buffer, GL_STATIC_DRAW));

	//setting up Element buffer object
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO));
	glCheck(glBufferDataFromVector(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer, GL_STATIC_DRAW));

	//position
	glCheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (GLvoid*)offsetof(TexturedVertex, position)));
	glCheck(glEnableVertexAttribArray(0));

	//texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (GLvoid*)offsetof(TexturedVertex, texture_coord));
	glCheck(glEnableVertexAttribArray(1));

	//normal
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (GLvoid*)offsetof(TexturedVertex, normal));
	glCheck(glEnableVertexAttribArray(2));


	//unbinding
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));

	glCheck(glBindVertexArray(0));
}

void Mesh::loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string type_name)
{
	int size = material->GetTextureCount(type);
	for (int i = 0; i < size; i++)
	{
		aiString name;
		material->GetTexture(type, i, &name);

		bool skip = false;
		for (auto loadedTexture : m_textures)
		{
			if (loadedTexture->getPath() == name.C_Str())
			{
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture* texture = new Texture(name.C_Str(), type_name.c_str());
			m_textures.push_back(texture);
		}

	}
}
