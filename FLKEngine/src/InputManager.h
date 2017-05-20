#pragma once

#include <unordered_map>
#include <glm\glm.hpp>

class InputManager
{
public:
	InputManager();
	~InputManager();

	void PressKey(unsigned int keyID);
	void ReleaseKey(unsigned int keyID);
	bool IsKeyDown(unsigned int keyID);

	//mouse funcs
	void SetMouseCoords(float x, float y);
	glm::vec2 GetMouseCoords() { return m_mouseCoords; }

private:
	std::unordered_map<unsigned int, bool>m_keyMap;
	glm::vec2 m_mouseCoords;

};

