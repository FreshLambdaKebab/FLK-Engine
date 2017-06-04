#include "Sprite.h"

Sprite::Sprite(const Shader& shader) :
	m_shader(shader)
{
	Init();
}

Sprite::~Sprite()
{
	glDeleteVertexArrays(1, &m_quadVAO);
}

void Sprite::Draw(const Texture2D& texture, glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec3 color)
{
	m_shader.Use();
	glm::mat4 model;
	//first transalte then scale
	model = glm::translate(model, glm::vec3(position, 0.0f));
	// move origin of rotation to center of quad
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); 
	//rotate
	model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); 
	// move origin back
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); 
	//finally scale
	model = glm::scale(model, glm::vec3(size, 1.0f));

	m_shader.SetMatrix4("model", model,GL_FALSE);

	//render the quad
	m_shader.SetVector3f("spriteColor", color,GL_FALSE);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	glBindVertexArray(m_quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Sprite::Init()
{
	GLuint VBO;

	GLfloat vertices[] = {
		// Position  // Texture
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &m_quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(m_quadVAO);
	//unbind arrays and buffers
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
