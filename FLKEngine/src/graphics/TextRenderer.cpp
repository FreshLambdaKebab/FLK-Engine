#include "TextRenderer.h"
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include <freetype\freetype.h>

#include "tools/ResourceManager.h"

TextRenderer::TextRenderer(GLuint width, GLuint height)
{
	//load and configure shader
	m_textShader = ResourceManager::LoadShader("res/shaders/textShader.vert", "res/shaders/textShader.frag", nullptr, "textShader");
	m_textShader.SetMatrix4("projection", glm::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f), GL_TRUE);
	m_textShader.SetInteger("text", 0);
	//configure VAO/VBO for quads
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

TextRenderer::~TextRenderer()
{
}

void TextRenderer::Load(const std::string & font, GLuint fontSize)
{
	//clear prev characters
	m_characters.clear();

	//init freetype
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR:FREETYPE: Could not initialize freetype!" << std::endl;
	}

	//load font as face
	FT_Face face;
	if (FT_New_Face(ft, font.c_str(), 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font!" << std::endl;
	}

	//set glyph size
	FT_Set_Pixel_Sizes(face, 0, fontSize);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//for first 128 ascii characters, pre-load characters
	for (GLubyte c = 0; c < 128; c++)
	{
		//load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: failed to laod glpyh" << std::endl;
			continue;
		}

		//generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		//set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};

		m_characters.insert(std::make_pair(c, character));
	}
	//unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
	//DESTROY freetype once done
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void TextRenderer::RenderText( std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	//activate corresponding render state
	m_textShader.Use();
	m_textShader.SetVector3f("textColor", color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_VAO);

	//iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = m_characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y + (m_characters['H'].Bearing.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;

		//update VBO for each character
		GLfloat vertices[6][4] =
		{
			{ xpos,     ypos + h,   0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 0.0 },
			{ xpos,     ypos,       0.0, 0.0 },

			{ xpos,     ypos + h,   0.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 1.0 },
			{ xpos + w, ypos,       1.0, 0.0 }
		};

		//render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		//update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//now advance cursors for next glyph
		x += (ch.Advance >> 6) * scale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
