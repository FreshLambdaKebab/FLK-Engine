#pragma once

#include <GL\glew.h>

class Texture2D
{
public:
	Texture2D();
	~Texture2D();

	//generate a texture from image data
	void Generate(const GLuint& width, const GLuint& height, unsigned char* data);
	void Bind()const;
	void Load(const GLchar* file,GLboolean alpha);

	//getters and setters
	GLuint& GetID(){ return m_ID; }
	GLuint GetInternalFormat() { return m_internalFormat; }
	GLuint GetImageFormat() { return m_imageFormat; }

	void SetInternalFormat(GLuint format) { m_internalFormat = format; }
	void SetImageFormat(GLuint format) { m_imageFormat = format; }

private:
	//every texture object has an ID, and also width and height
	GLuint m_ID;
	GLuint m_width, m_height;

	//texture format
	GLuint m_internalFormat;
	GLuint m_imageFormat;

	//textures configs
	GLuint m_wrapS;
	GLuint m_wrapT;
	GLuint m_filterMin;
	GLuint m_filterMax;

};

