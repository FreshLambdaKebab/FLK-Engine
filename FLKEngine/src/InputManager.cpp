#include "InputManager.h"

InputManager::InputManager() :
	m_mouseCoords(0.0f)
{
}

InputManager::~InputManager()
{
}

void InputManager::PressKey(unsigned int keyID)
{
	m_keyMap[keyID] = true;
}

void InputManager::ReleaseKey(unsigned int keyID)
{
	m_keyMap[keyID] = false;
}

bool InputManager::IsKeyDown(unsigned int keyID)
{
	auto it = m_keyMap.find(keyID);
	if (it != m_keyMap.end())
	{
		return it->second;
	}
	else
	{
		return false;
	}
}

void InputManager::SetMouseCoords(float x, float y)
{
	m_mouseCoords.x = x;
	m_mouseCoords.y = y;
}
