#include "RInputManager.h"


void RInputManager::Initialize()
{
	int i;

	// Initialize all the keys to being released and not pressed.
	for (i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}

	return;
}

//set pressed keys up.
void RInputManager::Reset()
{
	for(int key : m_pressedKeys)
	{
		m_keys[key] = false;
	}
	m_pressedKeys.clear();
}


void RInputManager::KeyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	m_keys[input] = true;
	m_pressedKeys.push_back(input);
	return;
}


void RInputManager::KeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	m_keys[input] = false;
	m_pressedKeys.push_back(input);
	return;
}


bool RInputManager::IsKeyDown(unsigned int key)
{
	auto val = m_keys[key];
	//m_keys[key] = false;
	// Return what state the key is in (pressed/not pressed).
	return val;
}

void RInputManager::SetMousePosition(int x, int y)
{
	MouseMove move;
	move.x = (x - m_mouseX);
	move.y = (y - m_mouseY);
	m_mouseMoves.push(move);
	m_mouseX = x;
	m_mouseY = y;
}
