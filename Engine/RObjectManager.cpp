#include "RObjectManager.h"
#include "RGameObject.h"
#include "RMesh.h"
#include "RMaterial.h"
#include "RResourceLoader.h"

void RObjectManager::Initialize()
{
	m_gameObjectPool.reserve(100);
}

void RObjectManager::Tick(float deltaTime)
{
	for(RGameObject* gameObject : m_gameObjectPool)
	{
		gameObject->Tick();
	}
	m_gameObjectPool[0]->SetPosition(2, 0, 10);
	static float rotX = 0;
	rotX += 100 *deltaTime;
	static float rotY = 0;
	rotY += 100 *deltaTime;
	m_gameObjectPool[0]->SetRotation(90, rotY, 0);

	m_gameObjectPool[1]->SetPosition(0, 0, 20);
	m_gameObjectPool[1]->SetRotation(90, rotY, 0);
}

void RObjectManager::AddGameObject(RGameObject* gameObject)
{
	m_gameObjectPool.push_back(gameObject);
}

void RObjectManager::RemoveGameObject(RGameObject* gameObject)
{
	for (auto it = m_gameObjectPool.begin(); it != m_gameObjectPool.end(); it++)
	{
		if (gameObject == *it)
			m_gameObjectPool.erase(it);
	}
}
