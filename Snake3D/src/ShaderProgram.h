#pragma once
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <GLM/glm.hpp>
#include <memory>
#include "Shader.h"

class ShaderProgram
{
public:
	ShaderProgram(std::shared_ptr<Shader> m_vertex_shader, std::shared_ptr<Shader> m_fragment_shader);
	~ShaderProgram();

	void bind()const;
	void unBind()const;
	void refresh();

	void setUniform1i(const char* uniform_name, int value);
	void setUniform1f(const char* uniform_name, float value);
	void setUniform4f(const char* uniform_name, float value1, float value2, float value3, float value4);
	void setUniform4fv(const char* uniform_name, glm::vec4 color);
	void setUniformMarix4fv(const char* uniform_name, glm::mat4 matrix);

	GLuint getProgram();
	static ShaderProgram* current_shader;
private:
	void init();
	int getUniformLocation(const char* name);

	std::shared_ptr<Shader> m_vertex_shader;
	std::shared_ptr<Shader> m_fragment_shader;

	GLuint m_program_ID;
};

