#pragma once
#include <vector>
#include <queue>
#include <set>
class RObjectManager
{
public:
	~RObjectManager();
	void Initialize();
	void Tick(double deltaTime);
	void AddGameObject(class RGameObject* gameObject);
	void AddPbrObject(class PbrObject* pbrObject);
	void RemovePbrObject(PbrObject* pbrObject);
	void RemoveGameObject(class RGameObject* gameObject);
	inline const std::vector<class RGameObject*>& GetGameObjects() { return m_gameObjects; }
	inline const std::vector<class PbrObject*>& GetPbrObjects() { return m_pbrObjects; }

private:
	std::vector<class RGameObject*> m_gameObjects;
	std::vector<class PbrObject*> m_pbrObjects;
};

extern RObjectManager* ObjectManager;
