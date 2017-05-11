#include <GL\glew.h>
#include <SDL\SDL.h>
#include <SDL\SDL_opengl.h>

#include "Shader.h"

const int SCREEN_WIDTH = 800; //without shader class: 158 lines of code
const int SCREEN_HEIGHT = 600;//with shader class: 120 lines of code
const std::string WINDOW_TITLE = "God damn fucking window";

int main(int argc,char* argv[])
{
	//std::cout << "Hello fuckwits of america!!!" << std::endl;
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
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
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
		0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
	};

	//upload and copy the vertex data 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//create an element array
	GLuint EBO;
	glGenBuffers(1, &EBO);
	
	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	//create and compile the shaders
	Shader colorShader;
	colorShader.Load("res/shaders/colorShader.vert", "res/shaders/colorShader.frag");

	//specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(colorShader.GetProgramID(), "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

	GLint colorAttrib = glGetAttribLocation(colorShader.GetProgramID(), "color");
	glEnableVertexAttribArray(colorAttrib);
	glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

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

		colorShader.Use();

		//draw the triangle from the 3 vertices
		glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT,0);

		SDL_GL_SwapWindow(window);
	}

	//delete arrays buffers and shaders
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);

	SDL_GL_DeleteContext(context);
	SDL_Quit();

	return 0;
}
