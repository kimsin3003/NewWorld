#include "GameManager.h"
#include "Engine/RContext.h"
#include "Room.h"

void GameManager::Initialize()
{
	RGameObject* room = new Room();
	std::vector<RTexture> textures;
	RTexture texture;
	texture.type = RTexture::DIFFUSE;
	texture.filename = L"../Resource/soccer_ball_diffuse.bmp";

	textures.push_back(texture);
	room->SetResource("ball.fbx");
	for (RMesh* mesh : room->Meshes)
	{
		RMaterial* defaultMaterial = new RMaterial(L"Default_VS.hlsl", L"Default_PS.hlsl", textures);
		mesh->Mat = defaultMaterial;
	}
	ObjectManager->AddGameObject(room);
	room->SetPosition(0, 0, 10);

// 	RGameObject* ball = new Room();
// 
// 	ball->SetResource("ball.fbx");
// 	for (RMesh* mesh : ball->Meshes)
// 	{
// 		RMaterial* defaultMaterial = new RMaterial(L"Default_VS.hlsl", L"Default_PS.hlsl", std::vector<RTexture>());
// 		mesh->Mat = defaultMaterial;
// 	}
// 	ObjectManager->AddGameObject(ball);
// 	ball->SetPosition(0, 0, 10);
}

void GameManager::Tick(double deltaTime)
{
}