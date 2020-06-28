#include "ShaderProgram.h"
#include <iostream>
#include <GLM/gtc/type_ptr.hpp>

ShaderProgram* ShaderProgram::current_shader = nullptr;

ShaderProgram::ShaderProgram(std::shared_ptr<Shader> vertex_shader, std::shared_ptr<Shader> fragment_shader)
	:m_fragment_shader(fragment_shader), m_vertex_shader(vertex_shader), m_program_ID(0)
{
	init();
	
}

ShaderProgram::~ShaderProgram()
{
	glCheck(glDeleteProgram(m_program_ID));
}

void ShaderProgram::bind() const
{
	glCheck(glUseProgram(m_program_ID));
	current_shader = const_cast<ShaderProgram*>(this);
}

void ShaderProgram::unBind() const
{
	glCheck(glUseProgram(0));
}

void ShaderProgram::refresh()
{
	m_vertex_shader->refresh();
	m_fragment_shader->refresh();
	init();
}

void ShaderProgram::setUniform1i(const char* uniform_name, int value)
{
	glCheck(glUniform1i(getUniformLocation(uniform_name), value));
}

void ShaderProgram::setUniform1f(const char* uniform_name, float value)
{
	glCheck(glUniform1f(getUniformLocation(uniform_name), value));
}

void ShaderProgram::setUniform4f(const char* uniform_name, float value1, float value2, float value3, float value4)
{
	glCheck(glUniform4f(getUniformLocation(uniform_name), value1, value2, value3, value4));
}

void ShaderProgram::setUniform4fv(const char* uniform_name, glm::vec4 color)
{
	glCheck(glUniform4fv(getUniformLocation(uniform_name), 1, (GLfloat*)&color));
}

void ShaderProgram::setUniformMarix4fv(const char* uniform_name, glm::mat4 matrix)
{
	glUniformMatrix4fv(getUniformLocation(uniform_name), 1, GL_FALSE, glm::value_ptr(matrix));
}

GLuint ShaderProgram::getProgram()
{
	return m_program_ID;
}

void ShaderProgram::init()
{
	m_program_ID = glCreateProgram();
	glCheck(glAttachShader(m_program_ID, m_vertex_shader->getShaderID()));
	glCheck(glAttachShader(m_program_ID, m_fragment_shader->getShaderID()));
	glCheck(glLinkProgram(m_program_ID));

	int success;
	glCheck(glGetProgramiv(m_program_ID, GL_LINK_STATUS, &success));
	if (!success)
	{
		char error_log[128];
		glCheck(glGetProgramInfoLog(m_program_ID, 128, nullptr, error_log));
		std::cerr << "Failed to link program! (" << error_log << ")\n";
	}
	current_shader = this;
}

int ShaderProgram::getUniformLocation(const char* name)
{
	int location = glGetUniformLocation(m_program_ID, name);
	if (location == -1)cout << "Warning: unifrom " << name << " probably doesn't exist!" << endl;
	//upgrade it
	return location;
}
