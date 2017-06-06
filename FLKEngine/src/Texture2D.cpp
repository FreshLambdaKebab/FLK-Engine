#include "Texture2D.h"

#include <SOIL\SOIL.h>

Texture2D::Texture2D() :
	m_width(0),
	m_height(0),
	m_internalFormat(GL_RGB),
	m_imageFormat(GL_RGB),
	m_wrapS(GL_REPEAT),
	m_wrapT(GL_REPEAT),
	m_filterMin(GL_LINEAR),
	m_filterMax(GL_LINEAR)
{
	glGenTextures(1, &m_ID);
}

Texture2D::~Texture2D()
{
}

void Texture2D::Generate(GLuint width,GLuint height, unsigned char * data)
{
	m_width = width;
	m_height = height;

	//create the texture
	glBindTexture(GL_TEXTURE_2D, m_ID);
	glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, width, height, 0, m_imageFormat, GL_UNSIGNED_BYTE, data);

	//set the texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filterMax);

	//unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind()const
{
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture2D::Load(const GLchar* file, GLboolean alpha)
{
	//check if alpha is on
	if (alpha)
	{
		m_internalFormat = GL_RGBA;
		m_imageFormat = GL_RGBA;
	}

	//load the image
	unsigned char* image = SOIL_load_image(file, (int*)&m_width, (int*)&m_height,0, m_imageFormat == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);

	//now generate the texture
	Generate(m_width, m_height, image);

	//free the image data when done
	SOIL_free_image_data(image);
}
