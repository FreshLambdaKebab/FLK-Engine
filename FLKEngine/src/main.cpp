#include <GL\glew.h>
#include <SDL\SDL.h>
#include <SDL\SDL_opengl.h>
#include <SOIL\SOIL.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Shader.h"
#include "Texture2D.h"
#include "Sprite.h"

//constants
const int SCREEN_WIDTH = 800; 
const int SCREEN_HEIGHT = 600;
const std::string WINDOW_TITLE = "God damn fucking window";

int main(int argc,char* argv[])
{
	//std::cout << "Hello fuckwits of america!!!" << std::endl;
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	//create sdl window
	SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_GLContext context = SDL_GL_CreateContext(window);
	
	//initialize glew
	glewExperimental = GL_TRUE;
	glewInit();

	//create a vertex array object
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	//create a vertex buffer object and copy the data to it
	GLuint VBO;
	glGenBuffers(1, &VBO);

	//setup vertices for triangle
	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left 
	};

	//upload and copy the vertex data 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//create an element array
	GLuint EBO;
	glGenBuffers(1, &EBO);
	
	GLuint indices[] = {
		0, 1, 3,//first triangle
		1, 2, 3//second triangle
	};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//create and compile the shaders
	Shader colorShader;
	colorShader.Load("res/shaders/colorShader.vert", "res/shaders/colorShader.frag");
	Shader spriteShader;
	spriteShader.Load("res/shaders/spriteShader.vert", "res/shaders/spriteShader.frag");
	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(SCREEN_WIDTH),
		static_cast<GLfloat>(SCREEN_HEIGHT), 0.0f, -1.0f, 1.0f);
	spriteShader.SetInteger("image", 0);
	spriteShader.SetMatrix4("projection", projection,GL_TRUE);

	//specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(colorShader.GetProgramID(), "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);

	GLint colorAttrib = glGetAttribLocation(colorShader.GetProgramID(), "color");
	glEnableVertexAttribArray(colorAttrib);
	glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	GLint texAttrib = glGetAttribLocation(colorShader.GetProgramID(), "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

	//load texture
	Texture2D texture;
	texture.Load("res/textures/awesomeface.png", GL_TRUE);
	
	//create sprite
	Sprite sprite(spriteShader);

	SDL_Event windowEvent;
	bool quit = false;
	//main loop
	while (!quit)
	{	
		while(SDL_PollEvent(&windowEvent))
		{
			switch (windowEvent.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_WINDOWEVENT_CLOSE:
				quit = true;
				break;
			case SDL_KEYDOWN:
				//exit the application if escape was pressed
				if (windowEvent.key.keysym.sym == SDLK_ESCAPE)
					quit = true;
				break;
			}
		}

		//do shit
		//clear the screen to a desired color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//bind teh texture
		//texture.Bind();

		//colorShader.Use();
		sprite.Draw(texture, glm::vec2(100, 100),glm::vec2(100,100));

		//draw the triangle from the 3 vertices
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		SDL_GL_SwapWindow(window);
	}

	//delete arrays buffers and shaders & textures
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);

	SDL_GL_DeleteContext(context);
	SDL_Quit();

	return 0;
}
