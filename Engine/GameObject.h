#pragma once

class GameObject
{
public:

	GameObject(int index) : m_index(index) {};
	virtual ~GameObject();

	virtual void Start();
	virtual void Tick();
	inline int GetIndex() { return m_index; }
	class Mesh* Mesh = nullptr;
private:
	int m_index;
};

