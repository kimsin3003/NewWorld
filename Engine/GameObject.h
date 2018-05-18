#pragma once

class GameObject
{
public:
	GameObject(int index) : m_index(index) {};
	virtual ~GameObject();

	inline int GetIndex() { return m_index; }
	class Mesh* Mesh = nullptr;
private:
	int m_index;
};

