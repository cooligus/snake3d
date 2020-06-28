#pragma once
#include <GL/glew.h>
#include <GL/glfw3.h>

#define TEXTURE_TYPE_DIFFUSE "diffuse"
#define TEXTURE_TYPE_SPECULAR "specular"

class Texture
{
public:
	Texture(const char* filename, const char* type);
	~Texture();

	void bind(unsigned int id = 0) const;
	void unBind() const;

	GLuint getTextureID();
	const char* getPath();
	const char* getType();

private:
	void loadAndCreate(const char* filename);

	const char* m_path;
	int m_width;
	int m_height;
	int m_BPP;
	unsigned char* m_local_buffer;
	GLuint m_texture_ID;
	const char* m_type;
};

