#pragma once
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <string>
#include "Debug.h"

using namespace std;

enum ShaderType
{
	VERTEX_SHADER = GL_VERTEX_SHADER,
	FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
};
class Shader
{
public:
	Shader(const char* filename, ShaderType shader_type, bool filename_is_shader = false);
	~Shader();

	GLuint getShaderID();
	void refresh();

private:
	void loadAndCreate(const char* filename, ShaderType shader_type);

	bool m_filename_is_shader;
	string m_filename;
	ShaderType m_shader_type;
	GLuint m_shader_ID;
};

