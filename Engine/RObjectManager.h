#pragma once
#include <vector>
#include <queue>
#include <set>
class RObjectManager
{
public:
	void Initialize();
	void Tick(double deltaTime);
	void AddGameObject(class RGameObject* gameObject);
	void RemoveGameObject(class RGameObject* gameObject);
	inline const std::vector<class RGameObject*>& GetGameObjectPool() { return m_gameObjectPool; }

private:
	std::vector<class RGameObject*> m_gameObjectPool;
};

