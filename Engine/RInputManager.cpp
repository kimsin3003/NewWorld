#include "RInputManager.h"


void RInputManager::Initialize()
{
	m_keyboard = std::make_unique<DirectX::Keyboard>();
	m_mouse = std::make_unique<DirectX::Mouse>();
}

bool RInputManager::IsKeyDown(Keyboard::Keys key)
{
	return m_keyboard->GetState().IsKeyDown(key);
}

DirectX::Mouse::State RInputManager::GetMouseState()
{
	auto state = m_mouse->GetState();
	return state;
}
