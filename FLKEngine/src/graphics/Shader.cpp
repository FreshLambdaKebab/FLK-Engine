#include "Shader.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

Shader::Shader() : 
	m_programID(0),
	m_vertexShaderID(0),
	m_fragmentShaderID(0)
{
}

Shader::~Shader()
{
}

void Shader::Compile(const GLchar * vertexSource, const GLchar * fragmentSource, const GLchar * geometrySource)
{
	GLuint sVertex, sFragment, gShader;
	//vertex shader
	sVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(sVertex, 1, &vertexSource, NULL);
	glCompileShader(sVertex);
	CheckCompileErrors(sVertex, "VERTEX");
	//fragment
	sFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sFragment, 1, &fragmentSource, NULL);
	glCompileShader(sFragment);
	CheckCompileErrors(sFragment, "FRAGMENT");

	//if geometry shader is given,also compile geometry shader
	if (geometrySource != nullptr)
	{
		gShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gShader, 1, &geometrySource, NULL);
		glCompileShader(gShader);
		CheckCompileErrors(gShader, "GEOMETRY");
	}
	m_programID = glCreateProgram();
	glAttachShader(m_programID, sVertex);
	glAttachShader(m_programID, sFragment);
	if (geometrySource != nullptr)
		glAttachShader(m_programID, gShader);
	glLinkProgram(m_programID);
	CheckCompileErrors(m_programID, "PROGRAM");
	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(sVertex);
	glDeleteShader(sFragment);
	if (geometrySource != nullptr)
		glDeleteShader(gShader);

}

void Shader::Load(const std::string & vShaderFile, const std::string & fShaderFile)
{
	std::string vertexCode;
	std::string fragmentCode;
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
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	
	//compiile the shader
	Compile(vShaderCode, fShaderCode);
}

Shader& Shader::Use()
{
	glUseProgram(m_programID);
	return *this;
}

void Shader::CheckCompileErrors(GLuint object, const std::string & type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
	else
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
}

