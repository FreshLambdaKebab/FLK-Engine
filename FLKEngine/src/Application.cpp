#include "Application.h"

#include "tools/ResourceManager.h"

//setup vertices for triangle
GLfloat vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
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

Application::Application() :
	m_camera(glm::vec3(0.0f, 0.0f, 3.0f)),
	m_lastX(m_screenWidth / 2),
	m_lastY(m_screenHeight / 2),
	m_fov(45.0f),
	m_dt(0.0f),
	m_lastFrame(0.0f),
	m_window(nullptr),
	//m_VAO(0),
	m_cubeVAO(0),
	m_lightVAO(0),
	m_VBO(0),
	m_EBO(0),
	m_lightPos(glm::vec3(1.2f,1.0f,2.0f))
{
}

Application::~Application()
{
	//clear all resources
	ResourceManager::Clear();

	//delete arrays buffers and shaders & textures
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
	//glDeleteVertexArrays(1, &m_VAO);
	glDeleteVertexArrays(1, &m_cubeVAO);

	SDL_GL_DeleteContext(m_context);
	SDL_Quit();
}

void Application::Run()
{
	//initialize everything
	Initialize();

	Loop();
}

void Application::Initialize()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	glfwInit();//only using glfw for timing (glfwgettime)

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	//create sdl window
	m_window = SDL_CreateWindow(m_windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_screenWidth, m_screenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	m_context = SDL_GL_CreateContext(m_window);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_CaptureMouse(SDL_TRUE);

	//initialize glew
	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_DEPTH_TEST);

	// Define the viewport dimensions
	glViewport(0, 0, m_screenWidth, m_screenHeight);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//create the cube VAO
	glGenVertexArrays(1, &m_cubeVAO);
	//create a vertex buffer object and copy the data to it
	glGenBuffers(1, &m_VBO);

	//upload and copy the vertex data 
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(m_cubeVAO);

	//specify the layout of the vertex data
	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//create the light VAO
	glGenVertexArrays(1, &m_lightVAO);
	glBindVertexArray(m_lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//create and compile the shaders
	ResourceManager::LoadShader("res/shaders/lightShader.vert", "res/shaders/lightShader.frag", nullptr, "lightShader");
	ResourceManager::LoadShader("res/shaders/lampShader.vert", "res/shaders/lampShader.frag", nullptr, "lampShader");

}

void Application::Loop()
{
	SDL_Event windowEvent;
	bool quit = false;
	//main loop
	while (!quit)
	{
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		m_dt = currentFrame - m_lastFrame;
		m_lastFrame = currentFrame;
		while (SDL_PollEvent(&windowEvent))
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
				m_inputManager.SetMouseCoords((float)windowEvent.motion.x, (float)windowEvent.motion.y);
				break;
			case SDL_KEYDOWN:
				m_inputManager.PressKey(windowEvent.key.keysym.sym);
				//exit the application if escape was pressed
				if (windowEvent.key.keysym.sym == SDLK_ESCAPE)
					quit = true;
				break;
			case SDL_KEYUP:
				m_inputManager.ReleaseKey(windowEvent.key.keysym.sym);
				break;

			}

		}

		//update input etc
		Update();

		//draw shit
		Render();

	}
}

void Application::Render()
{
	//do shit
	//clear the screen to a desired color
	glClearColor(0.2f, 0.4f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set uniforms and shit
	//activate light and lamp shaders
	ResourceManager::GetShader("lightShader").Use();
	ResourceManager::GetShader("lightShader").SetVector3f("objectColor",1.0f,0.5f,0.31f);
	ResourceManager::GetShader("lightShader").SetVector3f("lightColor", 1.0f, 1.0f, 1.0f);
	ResourceManager::GetShader("lightShader").SetVector3f("lightPos", m_lightPos);
	ResourceManager::GetShader("lightShader").SetVector3f("viewPos", m_camera.GetPosition());

	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(m_fov), static_cast<GLfloat>(m_screenWidth) / static_cast<GLfloat>(m_screenHeight), 0.1f, 100.0f);
	glm::mat4 view = m_camera.GetViewMatrix();
	ResourceManager::GetShader("lightShader").SetMatrix4("projection", projection, GL_TRUE);
	ResourceManager::GetShader("lightShader").SetMatrix4("view", view, GL_TRUE);

	// world transformation
	glm::mat4 model;
	ResourceManager::GetShader("lightShader").SetMatrix4("model", model,true);

	//firstly render the cube
	glBindVertexArray(m_cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//secondly setup & render the lamp object
	ResourceManager::GetShader("lampShader").Use();
	ResourceManager::GetShader("lampShader").SetMatrix4("projection", projection,true);
	ResourceManager::GetShader("lampShader").SetMatrix4("view", view,true);
	model = glm::mat4();
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
	ResourceManager::GetShader("lampShader").SetMatrix4("model", model,true);
	
	//draw light/lamp cube
	glBindVertexArray(m_lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	SDL_GL_SwapWindow(m_window);
}

void Application::Update()
{
	//move camera around
	DoMovement(m_inputManager);
	HandleMouse((float)m_inputManager.GetMouseCoords().x, (float)m_inputManager.GetMouseCoords().y);

	//move lights position over time
	m_lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
	m_lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
}

void Application::DoMovement(InputManager & input)
{
	//camera movement
	//keyboard input
	GLfloat cameraSpeed = 15.0f *m_dt;
	if (input.IsKeyDown(SDLK_w))
		m_camera.ProcessKeyboard(Cam_Movement::FORWARD, m_dt);
	if (input.IsKeyDown(SDLK_a))
		m_camera.ProcessKeyboard(Cam_Movement::LEFT, m_dt);
	if (input.IsKeyDown(SDLK_s))
		m_camera.ProcessKeyboard(Cam_Movement::BACKWARD, m_dt);
	if (input.IsKeyDown(SDLK_d))
		m_camera.ProcessKeyboard(Cam_Movement::RIGHT, m_dt);
}

bool firstMouse = true;
void Application::HandleMouse(int xpos, int ypos)
{
	SDL_GetMouseState(&xpos, &ypos);

	if (firstMouse)
	{
		m_lastX = xpos;
		m_lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - m_lastX;
	GLfloat yoffset = m_lastY - ypos;
	m_lastX = xpos;
	m_lastY = ypos;

	GLfloat sensitivity = 0.15f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	m_camera.ProcessMouse(xoffset, yoffset);
}
