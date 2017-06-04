#pragma once

#include <GL\glew.h>
#include <map>

#include "Texture2D.h"
#include "Shader.h"

class ResourceManager
{
public:
	~ResourceManager();

	//load and generate shader program from a file
	static Shader LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, const std::string& name);

	//get a stored shader
	static Shader GetShader(const std::string& name);

	//load and generate a texture from a file
	static Texture2D LoadTexture(const GLchar* file, GLboolean alpha, const std::string& name);

	//get a stored texture
	static Texture2D GetTexture(const std::string& name);

	//de-allocate all resources
	static void Clear();

private:
	ResourceManager();

	//load shaders and textures from files
	static Shader LoadShaderFromFile(const GLchar* vFileName, const GLchar* fFileName, const GLchar* gFileName = nullptr);
	static Texture2D LoadTextureFromFile(const GLchar* file, GLboolean alpha);

	//resource storage
	static std::map<std::string, Shader> m_shaders;
	static std::map<std::string, Texture2D>m_textures;
};

