#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject(std::vector<Vertex>* vertex_buffer, std::vector<GLuint>* element_buffer)
	:m_VAO(0), m_VBO(0), m_EBO(0), m_vertex_buffer(vertex_buffer), m_element_buffer(element_buffer)
{
	init();
}

VertexArrayObject::~VertexArrayObject()
{
	delete m_vertex_buffer;
	delete m_element_buffer;

	glCheck(glDeleteBuffers(1, &m_VBO));
	glCheck(glDeleteBuffers(1, &m_EBO));
	glCheck(glDeleteVertexArrays(1, &m_VAO));
}

void VertexArrayObject::draw(int amount_of_elements_to_draw) const
{
	bind();
	if (amount_of_elements_to_draw < 0)
	{
		amount_of_elements_to_draw = m_element_buffer->size();
	}
	if (amount_of_elements_to_draw > m_element_buffer->size())
	{
		amount_of_elements_to_draw = m_element_buffer->size();
	}
	glCheck(glDrawElements(GL_TRIANGLES, amount_of_elements_to_draw, GL_UNSIGNED_INT, nullptr));
	unBind();
}

void VertexArrayObject::bind() const
{
	glCheck(glBindVertexArray(m_VAO));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO));
}

void VertexArrayObject::unBind() const
{
	glCheck(glBindVertexArray(0));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void VertexArrayObject::changeVertices(std::vector<Vertex>* vertex_buffer)
{
	m_vertex_buffer = vertex_buffer;
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	glCheck(glBufferDataFromVec(GL_ARRAY_BUFFER, *m_vertex_buffer, GL_STATIC_DRAW));
}

void VertexArrayObject::changeIndices(std::vector<GLuint>* element_buffer)
{
	m_element_buffer = element_buffer;
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO));
	glCheck(glBufferDataFromVec(GL_ELEMENT_ARRAY_BUFFER, *m_element_buffer, GL_STATIC_DRAW));
}

GLuint VertexArrayObject::getVAO()
{
	return m_VAO;
}

GLuint VertexArrayObject::getVBO()
{
	return m_VBO;
}

GLuint VertexArrayObject::getEBO()
{
	return m_EBO;
}

void VertexArrayObject::init()
{
	glCheck(glGenVertexArrays(1, &m_VAO));
	glCheck(glGenBuffers(1, &m_VBO));
	glCheck(glGenBuffers(1, &m_EBO));

	glCheck(glBindVertexArray(m_VAO));

		//setting up Vertex buffer object
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
			glCheck(glBufferDataFromVec(GL_ARRAY_BUFFER, *m_vertex_buffer , GL_STATIC_DRAW));
		
		//setting up Element buffer object
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO));
			glCheck(glBufferDataFromVec(GL_ELEMENT_ARRAY_BUFFER, *m_element_buffer, GL_STATIC_DRAW));

		//position
		glCheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position)));
		glCheck(glEnableVertexAttribArray(0));
		
		//texture
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
		glCheck(glEnableVertexAttribArray(1));

		//unbinding
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));

	glCheck(glBindVertexArray(0));
}
