#pragma once

#include <GL\glew.h>
#include <SDL\SDL.h>
#include <GLFW\glfw3.h>
#include "InputManager.h"
#include "Camera.h"

class Application
{
public:
	Application();
	~Application();

	void Run();
	void Initialize();

private:
	void Update();
	void Render();
	void ProcessInput();

	void do_movement(InputManager& input);
	void mouse_callback(int xpos, int ypos);

	//just some constants
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

	//input manager
	InputManager m_inputManager;

	//sdl window pointer
	SDL_Window* m_window;
	SDL_GLContext m_context;

	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;
};

