#ifndef _SHADER_H
#define _SHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader
{
public:
	Shader();
	~Shader();

	//load and read shader from file and compile it 
	void Load(const std::string& vShaderFile,const std::string& fShaderFile);
	//compiles the shader from the given source
	void Compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource = nullptr);
	Shader& Use();

	//utility functions
	inline void SetFloat(const GLchar* name, GLfloat value, GLboolean useShader = false)
	{
		if (useShader)
			this->Use();

		glUniform1f(glGetUniformLocation(m_programID, name), value);
	}
	inline void SetInteger(const GLchar* name, GLfloat value, GLboolean useShader = false)
	{
		if (useShader)
			this->Use();

		glUniform1i(glGetUniformLocation(m_programID, name), value);
	}
	inline void SetVector2f(const GLchar* name, GLfloat x, GLfloat y, GLboolean useShader = false)
	{
		if (useShader)
			this->Use();

		glUniform2f(glGetUniformLocation(m_programID, name), x, y);
	}
	inline void SetVector2f(const GLchar* name, glm::vec2& value, GLboolean useShader = false)
	{
		if (useShader)
			this->Use();

		glUniform2f(glGetUniformLocation(m_programID, name), value.x, value.y);
	}
	inline void SetVector3f(const GLchar * name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false)
	{
		if (useShader)
			this->Use();

		glUniform3f(glGetUniformLocation(m_programID, name), x, y, z);
	}
	inline void SetVector3f(const GLchar * name, glm::vec3 & value, GLboolean useShader = false)
	{
		if (useShader)
			this->Use();

		glUniform3f(glGetUniformLocation(m_programID, name), value.x, value.y, value.z);
	}
	inline void SetVector4f(const GLchar * name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader =false)
	{
		if (useShader)
			this->Use();

		glUniform4f(glGetUniformLocation(m_programID, name), x, y, z, w);
	}
	inline void SetVector4f(const GLchar * name, glm::vec4 & value, GLboolean useShader)
	{
		if (useShader)
			this->Use();

		glUniform4f(glGetUniformLocation(m_programID, name), value.x, value.y, value.z, value.w);
	}
	inline void SetMatrix4(const GLchar * name, const glm::mat4 & matrix, GLboolean useShader)
	{
		if (useShader)
			this->Use();

		glUniformMatrix4fv(glGetUniformLocation(m_programID, name), 1, GL_FALSE, glm::value_ptr(matrix));
	}


	GLuint GetProgramID() const { return m_programID; }

private:
	void CheckCompileErrors(GLuint object, const std::string& type);

	GLuint m_programID;
	GLuint m_vertexShaderID;
	GLuint m_fragmentShaderID;
	GLuint m_geometryShaderID;

};

#endif // _SHADER_H
