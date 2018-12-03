#include "GameManager.h"
#include "Engine/RMaterial.h"
#include "Engine/RMesh.h"
#include "Engine/RObjectManager.h"
#include "Room.h"
#include "Engine/RLight.h"

void GameManager::Initialize()
{
// 	RGameObject* light = new RLight();
// 	light->Name = "Light";
// 	light->SetResource("ball.fbx");
// 	for (RMesh* mesh : light->Meshes)
// 	{
// 		RMaterial* defaultMaterial = new RMaterial(L"Default_VS.hlsl", L"Default_PS.hlsl", std::vector<RTexture>());
// 		mesh->Mat = defaultMaterial;
// 	}
// 	ObjectManager->AddGameObject(light);
// 	light->SetPosition(10, 10, 50);

	RGameObject* ball = new RGameObject();
	ball->Name = "Ball";
	ball->SetResource("ball.fbx");
	for (RMesh* mesh : ball->Meshes)
	{
		RMaterial* defaultMaterial = new RMaterial(L"Default_VS.hlsl", L"Default_PS.hlsl", std::vector<RTexture>());
		mesh->Mat = defaultMaterial;
	}
	ObjectManager->AddGameObject(ball);
	ball->SetPosition(0, 0, 50);
	ball->color = RVector3(0, 0.6, 0);

	RGameObject* room = new RGameObject();
	std::vector<RTexture> textures;
	RTexture texture;
	texture.type = RTexture::DIFFUSE;
	texture.filename = L"../Resource/zombie.fbm/world_war_zombie_diffuse.png";
	textures.push_back(texture);
	room->Name = "Room";
	room->SetResource("room.fbx");
	for (RMesh* mesh : room->Meshes)
	{
		RMaterial* defaultMaterial = new RMaterial(L"Default_VS.hlsl", L"Default_PS.hlsl", textures);
		mesh->Mat = defaultMaterial;
	}
	ObjectManager->AddGameObject(room);
	room->SetPosition(0, 0, 150);
	room->color = RVector3(0, 0, 0.6f);
}

void GameManager::Tick(double deltaTime)
{
}