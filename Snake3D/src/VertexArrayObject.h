#pragma once
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <GLM/glm.hpp>
#include <vector>
#include "Debug.h"

const float HEIGHT_OF_FLOATING_OF_OBJECTS = 0.05f;

template<typename T>
void glBufferDataFromVec(GLenum target, const std::vector<T> &buffer, GLenum type)
{	
	glCheck(glBufferData(target, sizeof(T) * buffer.size(), &buffer[0], type));
}

struct Vertex
{
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec4 color = glm::vec4(0.0f);
};

class VertexArrayObject
{
public:
	VertexArrayObject(std::vector<Vertex> *vertex_buffer, std::vector<GLuint> *element_buffer);
	~VertexArrayObject();

	//if amount_of_elements_to_draw < 0 amount_of_elements_to_draw equals size of element_buffer
	void draw(int amount_of_elements_to_draw = -1)const;
	void bind()const;
	void unBind()const;

	void changeVertices(std::vector<Vertex>* vertex_buffer);
	void changeIndices(std::vector<GLuint>* element_buffer);

	GLuint getVAO();
	GLuint getVBO();
	GLuint getEBO();

private:
	void init();

	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;

	std::vector<Vertex> *m_vertex_buffer;
	std::vector<GLuint> *m_element_buffer;
};

