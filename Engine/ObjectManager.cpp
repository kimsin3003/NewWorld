#include "ObjectManager.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Material.h"
#include "ResourceLoader.h"

void ObjectManager::Initialize()
{
	m_gameObjectPool.reserve(1000);
	for (int i = 0; i < 1000; i++)
	{
		m_gameObjectPool.emplace_back(new GameObject(i));
		m_indiciesNotOnUse.push(i);
	}
	TextureInfo info;
	info.filename = L"Resource/Sword.png";
	info.type = TextureInfo::TextureType::DIFFUSE;
	std::vector<TextureInfo> infos;
	infos.emplace_back(info);

	ResourceLoader loader;
	GameObject* gameObject1 = NewObject();
	loader.LoadFBX("Resource/Sword.FBX", gameObject1->Meshes);

	for (auto mesh : gameObject1->Meshes)
	{
		Material* defaultMaterial = new Material(L"Engine/Default_VS.hlsl", L"Engine/Default_PS.hlsl", infos);
		mesh->Mat = defaultMaterial;
	}
}

void ObjectManager::Tick(float deltaTime)
{
	for(int i : m_indiciesOnUse)
	{
		m_gameObjectPool[i]->Tick();
	}
	m_gameObjectPool[0]->SetPosition(0, 200, 500);
	static float rotX = 0;
	rotX += 100 *deltaTime;
	static float rotY = 0;
	rotY += 100 *deltaTime;
	m_gameObjectPool[0]->SetRotation(rotX, rotY, 0);
}

GameObject* const ObjectManager::NewObject()
{
	int indexToUse = m_indiciesNotOnUse.front();
	m_indiciesNotOnUse.pop();
	m_indiciesOnUse.insert(indexToUse);

	m_gameObjectPool[indexToUse]->Start();
	return m_gameObjectPool[indexToUse];
}

void ObjectManager::RemoveGameObject(GameObject* gameObject)
{
	m_indiciesOnUse.erase(gameObject->GetIndex());
	m_indiciesNotOnUse.push(gameObject->GetIndex());
}
