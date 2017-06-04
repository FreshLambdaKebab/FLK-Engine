#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <SOIL\SOIL.h>

//instantiate static variables
std::map<std::string, Shader>ResourceManager::m_shaders;
std::map<std::string, Texture2D>ResourceManager::m_textures;

ResourceManager::~ResourceManager()
{
}

Shader ResourceManager::LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, const std::string& name)
{
	m_shaders[name] = LoadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);

	return m_shaders[name];
}

Shader ResourceManager::GetShader(const std::string& name)
{
	return m_shaders[name];
}

Texture2D ResourceManager::LoadTexture(const GLchar * file, GLboolean alpha,const std::string& name)
{
	m_textures[name] = LoadTextureFromFile(file, alpha);

	return m_textures[name];
}

Texture2D ResourceManager::GetTexture(const std::string& name)
{
	return m_textures[name];
}

void ResourceManager::Clear()
{
	//properly delete all the shaders
	for (auto it : m_shaders)
		glDeleteProgram(it.second.GetProgramID());

	//properly delete all textures
	for (auto it : m_textures)
		glDeleteTextures(1, &it.second.GetID());
}

Shader ResourceManager::LoadShaderFromFile(const GLchar * vShaderFile, const GLchar * fShaderFile, const GLchar * gShaderFile)
{
	// 1. Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try
	{
		// Open files
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		// close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		// If geometry shader path is present, also load a geometry shader
		if (gShaderFile != nullptr)
		{
			std::ifstream geometryShaderFile(gShaderFile);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	const GLchar *gShaderCode = geometryCode.c_str();
	// 2. Now create shader object from source code
	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;
}

Texture2D ResourceManager::LoadTextureFromFile(const GLchar * file, GLboolean alpha)
{
	//create texture object
	Texture2D texture;

	if (alpha)
	{
		texture.SetInternalFormat(GL_RGBA);
		texture.SetImageFormat(GL_RGBA);
	}

	//load teh image
	int width, height;
	unsigned char* image = SOIL_load_image(file, &width, &height, 0, texture.GetImageFormat() == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);

	//generate texture
	texture.Generate(width, height, image);

	//free image data
	SOIL_free_image_data(image);

	return texture;
}

