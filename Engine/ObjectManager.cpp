#include "ObjectManager.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Material.h"
#include "ResourceLoader.h"

void ObjectManager::Initialize()
{
	ResourceLoader loader;
	std::vector<Vertex> outVertexVector;
	std::vector<unsigned int> outIndexVector;
	loader.LoadFBX("Resource/class2010.FBX", outVertexVector, outIndexVector);

	m_gameObjectPool.reserve(1000);
	for (int i = 0; i < 1000; i++)
	{
		m_gameObjectPool.emplace_back(new GameObject(i));
		m_indiciesNotOnUse.push(i);
	}
	GameObject* gameObject1 = NewObject();
	Mesh* triangleMesh = new Mesh();
	triangleMesh->SetData(outVertexVector, outIndexVector);
	TextureInfo info;
	info.filename = L"C:/Users/kimsi/Desktop/NewWorld/Resource/car.fbm/DRIVER.tga";
	info.type = TextureInfo::TextureType::DIFFUSE;
	std::vector<TextureInfo> infos;
	infos.emplace_back(info);
	Material* defaultMaterial = new Material(L"Engine/Default_VS.hlsl", L"Engine/Default_PS.hlsl", infos);
	triangleMesh->Mat = defaultMaterial;
	gameObject1->Mesh = triangleMesh;
}

void ObjectManager::Tick(float deltaTime)
{
	for(int i : m_indiciesOnUse)
	{
		m_gameObjectPool[i]->Tick();
	}
	m_gameObjectPool[0]->SetPosition(0, 0, 300);
	static float rotX = 0;
	rotX += 100 * deltaTime;
	static float rotY = 0;
	rotY += 100 * deltaTime;
	m_gameObjectPool[0]->SetRotation(rotX, 0, rotY);
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
