#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <map>

#include "Shader.h"
#include "Texture2D.h"

//character info
struct Character
{
	GLuint TextureID;//id of the glyph
	glm::ivec2 Size;//size of glyph
	glm::ivec2 Bearing;//offset to top left of glyph
	GLuint Advance;//horizontaly move to the next glyph
};

class TextRenderer
{
public:
	TextRenderer(GLuint width,GLuint height);
	~TextRenderer();

	//load a list of characters from a given font
	void Load(const std::string& font, GLuint fontSize);

	//render a string of text with the loaded font
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));

private:
	Shader m_textShader;
	std::map<GLchar, Character>m_characters;
	GLuint m_VAO, m_VBO;

};

