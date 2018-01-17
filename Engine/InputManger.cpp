#include "InputManger.h"

void InputManger::Initialize()
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
void InputManger::Reset()
{
	for(int key : m_pressedKeys)
	{
		m_keys[key] = false;
	}
	m_pressedKeys.clear();
}


void InputManger::KeyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	m_keys[input] = true;
	m_pressedKeys.push_back(input);
	return;
}


void InputManger::KeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	m_keys[input] = false;
	return;
}


bool InputManger::IsKeyDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
	return m_keys[key];
}