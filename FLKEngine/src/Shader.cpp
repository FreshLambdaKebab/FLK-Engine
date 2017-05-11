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

void Shader::Compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource)
{
	//vertex shader
	m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_vertexShaderID, 1, &vertexSource, NULL);
	glCompileShader(m_vertexShaderID);
	CheckCompileErrors(m_vertexShaderID, "VERTEX");

	//fragment
	m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_fragmentShaderID, 1, &fragmentSource, NULL);
	glCompileShader(m_fragmentShaderID);
	CheckCompileErrors(m_fragmentShaderID, "FRAGMENT");

	//if geometry shader is given,also compile geometry shader
	if (geometrySource != nullptr)
	{
		m_geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(m_geometryShaderID, 1, &geometrySource, NULL);
		glCompileShader(m_geometryShaderID);
		CheckCompileErrors(m_geometryShaderID, "GEOMETRY");
	}

	m_programID = glCreateProgram();
	glAttachShader(m_programID, m_vertexShaderID);
	glAttachShader(m_programID, m_fragmentShaderID);
	if (geometrySource != nullptr)
		glAttachShader(m_programID, m_geometryShaderID);
	glLinkProgram(m_programID);
	CheckCompileErrors(m_programID, "PROGRAM");

	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(m_vertexShaderID);
	glDeleteShader(m_fragmentShaderID);
	if (geometrySource != nullptr)
		glDeleteShader(m_geometryShaderID);

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

void Shader::Use()
{
	glUseProgram(m_programID);
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

