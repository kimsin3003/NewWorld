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
	for (RGameObject* gameObject : m_gameObjectPool)
	{
		gameObject->Tick(deltaTime);
	}
}

void RObjectManager::AddGameObject(RGameObject* gameObject)
{
	m_gameObjectPool.push_back(gameObject);
	gameObject->Start();
}

void RObjectManager::RemoveGameObject(RGameObject* gameObject)
{
	for (auto it = m_gameObjectPool.begin(); it != m_gameObjectPool.end(); it++)
	{
		if (gameObject == *it)
			m_gameObjectPool.erase(it);
	}
}
