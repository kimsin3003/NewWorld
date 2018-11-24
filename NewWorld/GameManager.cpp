#include "GameManager.h"
#include "Engine/RMaterial.h"
#include "Engine/RMesh.h"
#include "Engine/RObjectManager.h"
#include "Room.h"

void GameManager::Initialize()
{
	RGameObject* room = new Room();
	std::vector<RTexture> textures;
	RTexture texture;
	texture.type = RTexture::DIFFUSE;
	texture.filename = L"../Resource/zombie.fbm/world_war_zombie_diffuse.png";

	textures.push_back(texture);
	room->SetResource("room.fbx");
	for (RMesh* mesh : room->Meshes)
	{
		RMaterial* defaultMaterial = new RMaterial(L"Default_VS.hlsl", L"Default_PS.hlsl", textures);
		mesh->Mat = defaultMaterial;
	}
	ObjectManager->AddGameObject(room);
	room->SetPosition(0, 0, 150);

	RGameObject* ball = new Room();

	ball->SetResource("ball.fbx");
	for (RMesh* mesh : ball->Meshes)
	{
		RMaterial* defaultMaterial = new RMaterial(L"Default_VS.hlsl", L"Default_PS.hlsl", std::vector<RTexture>());
		mesh->Mat = defaultMaterial;
	}
	ObjectManager->AddGameObject(ball);
	ball->SetPosition(0, 0, 50);
}

void GameManager::Tick(double deltaTime)
{
}