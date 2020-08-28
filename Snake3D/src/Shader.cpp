#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>


Shader::Shader(const char* filename, ShaderType shader_type, bool filename_is_shader)
	:m_filename_is_shader(filename_is_shader), m_filename(filename), m_shader_ID(0), m_shader_type(shader_type)
{
	loadAndCreate(filename, shader_type);
}

Shader::~Shader()
{
	glCheck(glDeleteShader(m_shader_ID));
}

GLuint Shader::getShaderID()
{
	return m_shader_ID;
}

void Shader::refresh()
{
	if(!m_filename_is_shader)
	loadAndCreate(m_filename.c_str(), m_shader_type);
}

void Shader::loadAndCreate(const char* filename, ShaderType shader_type)
{
	m_filename = filename;
	if (!m_filename_is_shader)
	{
		ifstream stream;
		stream.open(filename);
		if (!stream.good())
		{
			cerr << "Failed to open shader file!\n";
			return;
		}

		stringstream sstream;
		sstream << stream.rdbuf();

		stream.close();

		string shader_source_str = sstream.str();
		const char* shader_source = shader_source_str.c_str();

		m_shader_type = shader_type;
		m_shader_ID = glCreateShader(shader_type);

		glCheck(glShaderSource(m_shader_ID, 1, &shader_source, nullptr));
		glCheck(glCompileShader(m_shader_ID));

		int success;
		glCheck(glGetShaderiv(m_shader_ID, GL_COMPILE_STATUS, &success));

		if (!success)
		{
			char error_log[128];
			glCheck(glGetShaderInfoLog(m_shader_ID, 128, nullptr, error_log));
			cerr << "Failed to compile shader shader from: " << filename << " (" << error_log << ")\n";
		}
	}
	else
	{
		m_shader_type = shader_type;
		m_shader_ID = glCreateShader(shader_type);

		glCheck(glShaderSource(m_shader_ID, 1, &filename, nullptr));
		glCheck(glCompileShader(m_shader_ID));

		int success;
		glCheck(glGetShaderiv(m_shader_ID, GL_COMPILE_STATUS, &success));

		if (!success)
		{
			char error_log[128];
			glCheck(glGetShaderInfoLog(m_shader_ID, 128, nullptr, error_log));
			cerr << "Failed to compile shader shader from: " << filename << " (" << error_log << ")\n";
		}
	}
}
