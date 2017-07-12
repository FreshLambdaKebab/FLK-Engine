#ifndef _CAMERA_H
#define _CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum class Cam_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 10.0f;
const GLfloat SENSITIVTY = 0.5f;

class Camera
{
public:
	Camera(glm::vec3 pos = glm::vec3(0.0f,0.0f,0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);
	~Camera();

	const glm::mat4 GetViewMatrix()const
	{
		return glm::lookAt(m_position, m_position + m_front, m_up);
	}

	glm::vec3 GetPosition()const { return m_position; }

	void ProcessKeyboard(const Cam_Movement& dir, GLfloat dt);
	void ProcessMouse(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

private:
	//calculate the front vector from eular angles
	void Update();

	//cam attribs
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;

	//eular angles 
	GLfloat m_yaw;
	GLfloat m_pitch;

	//cam options
	GLfloat m_movementSpeed;
	GLfloat m_mouseSensitivity;

};

#endif//_CAMERA_H
