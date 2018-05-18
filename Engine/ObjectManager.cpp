#include "ObjectManager.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Material.h"

void ObjectManager::Start()
{
	m_gameObjectPool.reserve(1000);
	for (int i = 0; i < 1000; i++)
	{
		m_gameObjectPool.emplace_back(new GameObject(i));
		m_indiciesNotOnUse.push(i);
	}
	GameObject* const gameObject1 = NewObject();
	Mesh* triangleMesh = new Mesh();
	Material* defaultMaterial = new Material(L"Engine/Default_VS.hlsl", L"Engine/Default_PS.hlsl");
	triangleMesh->Mat = defaultMaterial;
	gameObject1->Mesh = triangleMesh;
}

void ObjectManager::Tick(float deltaTime)
{

}

GameObject* const ObjectManager::NewObject()
{
	int indexToUse = m_indiciesNotOnUse.front();
	m_indiciesNotOnUse.pop();
	m_indiciesOnUse.insert(indexToUse);
	return m_gameObjectPool[indexToUse];
}

void ObjectManager::RemoveGameObject(GameObject* gameObject)
{
	m_indiciesOnUse.erase(gameObject->GetIndex());
	m_indiciesNotOnUse.push(gameObject->GetIndex());
}
