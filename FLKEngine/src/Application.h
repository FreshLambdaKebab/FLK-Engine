#pragma once

#include <GL\glew.h>
#include <SDL\SDL.h>
#include <GLFW\glfw3.h>
#include "IO/InputManager.h"
#include "tools/Camera.h"

class Application
{
public:
	Application();
	~Application();

	//initialize and update everything
	void Run();

private:
	void Initialize();//initialize and load textures,sprites etc
	void Loop();
	void Render();
	void Update();

	void DoMovement(InputManager& input);
	void HandleMouse(int xpos, int ypos);

private:

	//constants
	const int m_screenWidth = 800;
	const int m_screenHeight = 600;
	const std::string m_windowTitle = "God damn fucking window";

	//camera
	Camera m_camera;
	GLfloat m_lastX, m_lastY;
	GLfloat m_fov;

	//delta time
	GLfloat m_dt;
	GLfloat m_lastFrame;

	//input
	InputManager m_inputManager;

	//sdl window pointer
	SDL_Window* m_window;
	SDL_GLContext m_context;

	//GLuint m_VAO;
	GLuint m_cubeVAO;
	GLuint m_lightVAO;
	GLuint m_VBO;
	GLuint m_EBO;
};

