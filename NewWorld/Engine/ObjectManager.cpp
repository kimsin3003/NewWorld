#include "ObjectManager.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Material.h"
#include "Share/JsonUtility.h"

void ObjectManager::Initialize()
{
	m_gameObjectPool.reserve(1000);
	for (int i = 0; i < 1000; i++)
	{
		m_gameObjectPool.emplace_back(new GameObject(i));
		m_indiciesNotOnUse.push(i);
	}
	TextureInfo info;
	info.filename = L"../Resource/warzombie_f_pedroso.fbm/world_war_zombie_diffuse.png";
	info.type = TextureInfo::TextureType::DIFFUSE;
	std::vector<TextureInfo> texture;
	texture.emplace_back(info);

	GameObject* gameObject1 = NewObject();
	ModelInfo modelInfo;
	JsonUtility::ReadModelFromFile(&modelInfo, "../Resource/zombie.json");

	for (auto& meshInfo : modelInfo.meshInfos)
	{
		Mesh* mesh = new Mesh();
		mesh->SetData(meshInfo);
		Material* defaultMaterial = new Material(L"Engine/Default_VS.hlsl", L"Engine/Default_PS.hlsl", texture);
		mesh->Mat = defaultMaterial;
		gameObject1->Meshes.push_back(mesh);
	}
}

void ObjectManager::Tick(float deltaTime)
{
	for(int i : m_indiciesOnUse)
	{
		m_gameObjectPool[i]->Tick();
	}
	m_gameObjectPool[0]->SetPosition(0, -100, 500);
	static float rotX = 0;
	rotX += 100 *deltaTime;
	static float rotY = 0;
	rotY += 100 *deltaTime;
	m_gameObjectPool[0]->SetRotation(90, 180, 0);
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
