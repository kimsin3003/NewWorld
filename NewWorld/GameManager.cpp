#include "GameManager.h"
#include "Engine/RContext.h"
#include "Room.h"

void GameManager::Initialize()
{
// 	RGameObject* room = new Room();
// 
// 	room->SetResource("room.fbx");
// 	for (RMesh* mesh : room->Meshes)
// 	{
// 		RMaterial* defaultMaterial = new RMaterial(L"Default_VS.hlsl", L"Default_PS.hlsl", std::vector<RTexture>());
// 		mesh->Mat = defaultMaterial;
// 	}
// 	ObjectManager->AddGameObject(room);
// 	room->SetPosition(0, 0, 200);

	RGameObject* ball = new Room();

	ball->SetResource("ball.fbx");
	for (RMesh* mesh : ball->Meshes)
	{
		RMaterial* defaultMaterial = new RMaterial(L"Default_VS.hlsl", L"Default_PS.hlsl", std::vector<RTexture>());
		mesh->Mat = defaultMaterial;
	}
	ObjectManager->AddGameObject(ball);
	ball->SetPosition(0, 0, 10);
}

void GameManager::Tick()
{
}