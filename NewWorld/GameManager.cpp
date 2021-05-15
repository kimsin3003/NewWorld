#include "GameManager.h"
#include "Engine/RContext.h"
#include "Room.h"

void GameManager::Initialize()
{
	RGameObject* room = new Room();
// 	std::vector<RTexture> textures;
// 	RTexture texture;
// 	texture.type = RTexture::DIFFUSE;
// 	texture.filename = L"../Resource/Spider Man HC.fbm/body_d.png";
// 
// 	textures.push_back(texture);
	room->SetResource("spiderman.fbx");
// 	for (RMesh* mesh : room->Meshes)
// 	{
// 		RMaterial* defaultMaterial = new RMaterial(L"Default_VS.hlsl", L"Default_PS.hlsl", textures);
// 		mesh->Mat = defaultMaterial;
// 	}
	ObjectManager->AddGameObject(room);
	room->SetPosition(0, 0, 200);
}

void GameManager::Tick(double deltaTime)
{
	m_camMover.Tick(deltaTime);
}