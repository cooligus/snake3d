#include "Texture.h"
#include "Debug.h"
#include <STB/stb_image.h>
#include <STB/stb_image_implementation.h>

Texture::Texture(const char* filename, const char* type)
	:m_type(type), m_path(filename), m_width(0), m_height(0), m_BPP(0), m_local_buffer(0), m_texture_ID(0)
{
	loadAndCreate(filename);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_texture_ID);
}

void Texture::bind(unsigned int id) const
{
	glCheck(glActiveTexture(GL_TEXTURE0+id));
	glCheck(glBindTexture(GL_TEXTURE_2D, m_texture_ID));
}

void Texture::unBind() const
{
	glCheck(glBindTexture(GL_TEXTURE_2D, 0));
}

GLuint Texture::getTextureID()
{
	return m_texture_ID;
}

const char* Texture::getPath()
{
	return m_path;
}

const char* Texture::getType()
{
	return m_type;
}

void Texture::loadAndCreate(const char* filename)
{
	m_path = filename;

	stbi_set_flip_vertically_on_load(1);
	m_local_buffer = stbi_load(filename, &m_width, &m_height, &m_BPP, 4);
	
	glCheck(glCreateTextures(GL_TEXTURE_2D, 1, &m_texture_ID));
	glCheck(glBindTexture(GL_TEXTURE_2D, m_texture_ID));

	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));

	glCheck(glTexImage2D(GL_TEXTURE_2D, 0.0, GL_RGBA8, m_width, m_height, 0.0, GL_RGBA, GL_UNSIGNED_BYTE, m_local_buffer));
	glCheck(glGenerateMipmap(GL_TEXTURE_2D));

	glCheck(glBindTexture(GL_TEXTURE_2D, 0));

	if(m_local_buffer)stbi_image_free(m_local_buffer);

	if (m_width == 0 && m_height == 0)
		cout << "Probably something went wrong with loading texture from file " << filename <<".\n";
}
