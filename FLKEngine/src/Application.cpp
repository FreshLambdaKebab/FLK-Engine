#include "Application.h"

#include "ResourceManager.h"

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

Application::Application() :
	m_camera(glm::vec3(0.0f, 0.0f, 1.0f)),
	m_lastX(m_screenWidth / 2),
	m_lastY(m_screenHeight / 2),
	m_fov(45.0f),
	m_dt(0.0f),
	m_lastFrame(0.0f),
	m_window(nullptr),
	m_VAO(0),
	m_VBO(0),
	m_EBO(0)
{
}

Application::~Application()
{
	ResourceManager::Clear();

	//delete arrays buffers and shaders & textures
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
	glDeleteVertexArrays(1, &m_VAO);

	SDL_GL_DeleteContext(m_context);
	SDL_Quit();
}

void Application::Run()
{
	//initialize everything
	Initialize();

	Update();
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

	//create a vertex array object
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	//create a vertex buffer object and copy the data to it
	glGenBuffers(1, &m_VBO);

	//upload and copy the vertex data 
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//create an element array
	glGenBuffers(1, &m_EBO);

	GLuint indices[] = {
		0, 1, 3,//first triangle
		1, 2, 3//second triangle
	};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
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
	ResourceManager::LoadTexture("res/textures/bg.jpeg", GL_FALSE, "bill");

}

void Application::Update()
{
	//handle events
	ProcessInput();
}

void Application::Render()
{
	//do shit
	//clear the screen to a desired color
	glClearColor(0.2f, 0.8f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//bind teh texture
	//texture.Bind();
	ResourceManager::GetTexture("bill").Bind();
	ResourceManager::GetShader("colorShader").Use();

	//create camera/view transformations
	glm::mat4 view = m_camera.GetViewMatrix();

	//projection
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(m_fov), static_cast<GLfloat>(m_screenWidth) / static_cast<GLfloat>(m_screenHeight), 0.1f, 100.0f);
	//get the uniform locations & pass them to the shader
	ResourceManager::GetShader("colorShader").SetMatrix4("view", view, GL_TRUE);
	ResourceManager::GetShader("colorShader").SetMatrix4("projection", projection, GL_TRUE);

	//move camera around
	do_movement(m_inputManager);
	mouse_callback((float)m_inputManager.GetMouseCoords().x, (float)m_inputManager.GetMouseCoords().y);

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

	SDL_GL_SwapWindow(m_window);
}

void Application::ProcessInput()
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

		//draw shit
		Render();

	}
}

void Application::do_movement(InputManager & input)
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
void Application::mouse_callback(int xpos, int ypos)
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