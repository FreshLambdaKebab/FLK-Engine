#include "Camera.h"

Camera::Camera(glm::vec3 pos, glm::vec3 up, GLfloat yaw, GLfloat pitch) :
	m_position(pos),
	m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
	m_worldUp(up),
	m_yaw(yaw),
	m_pitch(pitch),
	m_movementSpeed(SPEED),
	m_mouseSensitivity(SENSITIVTY)
{
	Update();
}


Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) :
	m_position(glm::vec3(posX, posY, posZ)),
	m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
	m_worldUp(glm::vec3(glm::vec3(upX, upY, upZ))),
	m_yaw(yaw),
	m_pitch(pitch),
	m_movementSpeed(SPEED),
	m_mouseSensitivity(SENSITIVTY)
{
	Update();
}

Camera::~Camera()
{
}

void Camera::ProcessKeyboard(const Cam_Movement& dir, GLfloat dt)
{
	GLfloat velocity = m_movementSpeed * dt;
	if (dir == Cam_Movement::FORWARD)
		m_position += m_front * velocity;
	if (dir == Cam_Movement::BACKWARD)
		m_position -= m_front * velocity;
	if (dir == Cam_Movement::LEFT)
		m_position -= m_right * velocity;
	if (dir == Cam_Movement::RIGHT)
		m_position += m_right * velocity;
}

void Camera::ProcessMouse(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
	xoffset *= m_mouseSensitivity;
	yoffset *= m_mouseSensitivity;

	m_yaw += xoffset;
	m_pitch += yoffset;

	//dont let screen flip
	if (constrainPitch)
	{
		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = -89.0f;
	}

	Update();
}

void Camera::Update()
{
	//calculate the front vector
	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);
	// re calculate up and right vectors
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}
