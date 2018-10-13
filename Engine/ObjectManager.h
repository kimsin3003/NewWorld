#pragma once
#include <vector>
#include <queue>
#include <set>
class ObjectManager
{
public:
	void Initialize();
	void Tick(float deltaTime);
	class GameObject* const NewObject();
	void RemoveGameObject(class GameObject* gameObject);
	inline const std::vector<class GameObject*>& GetGameObjectPool() { return m_gameObjectPool; }
	inline const std::set<int>& GetIndiciesOnUse() { return m_indiciesOnUse; }

private:
	std::vector<class GameObject*> m_gameObjectPool;
	std::queue<int> m_indiciesNotOnUse;
	std::set<int> m_indiciesOnUse;
};

