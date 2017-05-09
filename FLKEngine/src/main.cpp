#include <GL\glew.h>
#include <SDL\SDL.h>
#include <SDL\SDL_opengl.h>
#include <stdio.h>

int main(int argc,char* argv[])
{
	//std::cout << "Hello fuckwits of america!!!" << std::endl;
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	//create sdl window
	SDL_Window* window = SDL_CreateWindow("God damn fucking window", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);
	
	//initialize glew
	glewExperimental = GL_TRUE;
	glewInit();

	//testing
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);

	printf("%u\n", vertexBuffer);

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
		
		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
	SDL_Quit();

	return 0;
}
