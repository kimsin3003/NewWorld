#include "GameManager.h"
#include "Engine/RContext.h"
#include "Room.h"

void GameManager::Initialize()
{
	RGameObject* model = new Room();
// 	std::vector<RTexture> textures;
// 	RTexture texture;
// 	texture.type = RTexture::DIFFUSE;
// 	texture.filename = L"../Resource/Spider Man HC.fbm/body_d.png";
// 
// 	textures.push_back(texture);
	model->SetResource("../Resource/paladin_j_nordstrom.fbx");
// 	for (RMesh* mesh : room->Meshes)
// 	{
// 		RMaterial* defaultMaterial = new RMaterial(L"Default_VS.hlsl", L"Default_PS.hlsl", textures);
// 		mesh->Mat = defaultMaterial;
// 	}
	ObjectManager->AddGameObject(model);
	model->SetPosition(0, -100, 200);
	model->SetRotation(0, 180, 0);
}

void GameManager::Tick(double deltaTime)
{
	m_camMover.Tick(deltaTime);
}