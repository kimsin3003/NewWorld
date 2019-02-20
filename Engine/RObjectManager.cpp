#include "RObjectManager.h"
#include "RGameObject.h"
#include "PbrObject.h"
#include "RMesh.h"
#include "RMaterial.h"
RObjectManager* ObjectManager = new RObjectManager();

RObjectManager::~RObjectManager()
{
	for (auto object : m_gameObjects)
	{
		delete object;
	}
	m_gameObjects.clear();
}

void RObjectManager::Initialize()
{
	m_gameObjects.reserve(20);
}

void RObjectManager::Tick(double deltaTime)
{
	for (RGameObject* gameObject : m_gameObjects)
	{
		gameObject->Tick(deltaTime);
	}
}

void RObjectManager::AddGameObject(RGameObject* gameObject)
{
	m_gameObjects.push_back(gameObject);
	gameObject->Start();
}

void RObjectManager::AddPbrObject(class PbrObject* pbrObject)
{
	m_pbrObjects.push_back(pbrObject);
}

void RObjectManager::RemovePbrObject(PbrObject* pbrObject)
{
	for (auto it = m_pbrObjects.begin(); it != m_pbrObjects.end(); it++)
	{
		if (pbrObject == *it)
			m_pbrObjects.erase(it);
	}
}

void RObjectManager::RemoveGameObject(RGameObject* gameObject)
{
	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		if (gameObject == *it)
			m_gameObjects.erase(it);
	}
}
