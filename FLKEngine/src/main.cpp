#include <GL\glew.h>
#include <SDL\SDL.h>
#include <SDL\SDL_opengl.h>
#include <SOIL\SOIL.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GLFW\glfw3.h>

#include "Texture2D.h"
#include "InputManager.h"
#include "Camera.h"
#include "TextRenderer.h"
#include "ResourceManager.h"

//constants
const int SCREEN_WIDTH = 800; 
const int SCREEN_HEIGHT = 600;
const std::string WINDOW_TITLE = "God damn fucking window";

//camera
Camera camera(glm::vec3(0.0f, 0.0f, 1.0f));
GLfloat lastX = SCREEN_WIDTH / 2;
GLfloat lastY = SCREEN_HEIGHT / 2;
GLfloat fov = 45.0f;
bool keys[1024];

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

void DoMovement(InputManager& input);
void mouse_callback(int xpos,int ypos);

int main(int argc,char* argv[])
{
	//std::cout << "Hello fuckwits of america!!!" << std::endl;
	SDL_Init(SDL_INIT_EVERYTHING);
	glfwInit();//only using glfw for timing (glfwgettime)

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	//create sdl window
	SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_GLContext context = SDL_GL_CreateContext(window);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_CaptureMouse(SDL_TRUE);

	//initialize glew
	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_DEPTH_TEST);

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//create a vertex array object
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	//create a vertex buffer object and copy the data to it
	GLuint VBO;
	glGenBuffers(1, &VBO);

	//setup vertices for triangle
	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
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
	ResourceManager::LoadShader("res/shaders/colorShader.vert", "res/shaders/colorShader.frag", nullptr, "colorShader");

	//specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(ResourceManager::GetShader("colorShader").GetProgramID(), "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);

	GLint colorAttrib = glGetAttribLocation(ResourceManager::GetShader("colorShader").GetProgramID(), "color");
	glEnableVertexAttribArray(colorAttrib);
	glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	GLint texAttrib = glGetAttribLocation(ResourceManager::GetShader("colorShader").GetProgramID(), "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	//load texture
	//ResourceManager::LoadTexture("res/textures/awesomeface.png", GL_TRUE, "face");
	Texture2D texture;
	texture.Load("res/textures/bg.jpeg",GL_FALSE);

	InputManager inputManager;

	SDL_Event windowEvent;
	bool quit = false;
	//main loop
	while (!quit)
	{	
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
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
			case SDL_MOUSEMOTION:
				inputManager.SetMouseCoords((float)windowEvent.motion.x, (float)windowEvent.motion.y);
				break;
			case SDL_KEYDOWN:
				inputManager.PressKey(windowEvent.key.keysym.sym);
				//exit the application if escape was pressed
				if (windowEvent.key.keysym.sym == SDLK_ESCAPE)
					quit = true;
				break;
			case SDL_KEYUP:
				inputManager.ReleaseKey(windowEvent.key.keysym.sym);
				break;
			
			}

		}

		//do shit
		//clear the screen to a desired color
		glClearColor(0.2f, 0.8f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//bind teh texture
		texture.Bind();
		//ResourceManager::GetTexture("face").Bind();
		ResourceManager::GetShader("colorShader").Use();

		//create camera/view transformations
		glm::mat4 view = camera.GetViewMatrix();
		
		//projection
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(fov), static_cast<GLfloat>(SCREEN_WIDTH) / static_cast<GLfloat>(SCREEN_HEIGHT), 0.1f, 100.0f);
		//get the uniform locations & pass them to the shader
		ResourceManager::GetShader("colorShader").SetMatrix4("view", view, GL_TRUE);
		ResourceManager::GetShader("colorShader").SetMatrix4("projection", projection, GL_TRUE);

		//move camera around
		DoMovement(inputManager);
		mouse_callback((float)inputManager.GetMouseCoords().x, (float)inputManager.GetMouseCoords().y);

		for (GLuint i = 0; i < 10; i++)
		{
			//calculate the model matrix for each object
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = 20.0f * i;
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			ResourceManager::GetShader("colorShader").SetMatrix4("model", model, GL_TRUE);

			//draw the triangle from the 3 vertices
			glDrawArrays(GL_TRIANGLES, 0, 36);

		}

		SDL_GL_SwapWindow(window);
	}

	ResourceManager::Clear();

	//delete arrays buffers and shaders & textures
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);

	SDL_GL_DeleteContext(context);
	SDL_Quit();

	return 0;
}

void DoMovement( InputManager& input)
{
	//camera movement
	//keyboard input
	GLfloat cameraSpeed = 15.0f *deltaTime;
	if (input.IsKeyDown(SDLK_w))
		camera.ProcessKeyboard(Cam_Movement::FORWARD, deltaTime);
	if (input.IsKeyDown(SDLK_a))
		camera.ProcessKeyboard(Cam_Movement::LEFT, deltaTime);
	if (input.IsKeyDown(SDLK_s))
		camera.ProcessKeyboard(Cam_Movement::BACKWARD, deltaTime);
	if (input.IsKeyDown(SDLK_d))
		camera.ProcessKeyboard(Cam_Movement::RIGHT, deltaTime);
}

bool firstMouse = true;
void mouse_callback( int xpos, int ypos)
{
	SDL_GetMouseState(&xpos, &ypos);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.15f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	camera.ProcessMouse(xoffset, yoffset);
}

