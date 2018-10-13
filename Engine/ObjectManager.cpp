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
// 	TextureInfo info;
// 	info.filename = L"../Resource/remy.fbm/Remy_Body_Diffuse.png";
// 	info.type = TextureInfo::TextureType::DIFFUSE;
// 	std::vector<TextureInfo> texture;
// 	texture.emplace_back(info);

	GameObject* gameObject1 = NewObject();
	ModelInfo modelInfo;
	JsonUtility::ReadModelFromFile(&modelInfo, "../Resource/rectangle.json");

	for (auto& meshInfo : modelInfo.meshInfos)
	{
		Mesh* mesh = new Mesh();
		mesh->SetData(meshInfo);
		Material* defaultMaterial = new Material(L"Default_VS.hlsl", L"Default_PS.hlsl", std::vector<TextureInfo>());
		mesh->Mat = defaultMaterial;
		gameObject1->Meshes.push_back(mesh);
	}


	GameObject* gameObject2 = NewObject();
	ModelInfo modelInfo2;
	JsonUtility::ReadModelFromFile(&modelInfo2, "../Resource/room.json");

	for (auto& meshInfo : modelInfo2.meshInfos)
	{
		Mesh* mesh = new Mesh();
		mesh->SetData(meshInfo);
		Material* defaultMaterial = new Material(L"Default_VS.hlsl", L"Default_PS.hlsl", std::vector<TextureInfo>());
		mesh->Mat = defaultMaterial;
		gameObject2->Meshes.push_back(mesh);
	}
}

void ObjectManager::Tick(float deltaTime)
{
	for(int i : m_indiciesOnUse)
	{
		m_gameObjectPool[i]->Tick();
	}
	m_gameObjectPool[0]->SetPosition(20, 0, 10);
	static float rotX = 0;
	rotX += 10 *deltaTime;
	static float rotY = 0;
	rotY += 10 *deltaTime;
	m_gameObjectPool[0]->SetRotation(90, rotY, 0);

	m_gameObjectPool[1]->SetPosition(0, 0, 20);
	m_gameObjectPool[1]->SetRotation(90, rotY, 0);
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
