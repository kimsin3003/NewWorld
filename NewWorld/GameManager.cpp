#include "GameManager.h"
#include "Engine/RContext.h"
#include "Room.h"

void GameManager::Initialize()
{
	RGameObject* gameObject = new Room();

	gameObject->SetResource("rectangle.FBX");
	for (RMesh* mesh : gameObject->Meshes)
	{
		RMaterial* defaultMaterial = new RMaterial(L"Default_VS.hlsl", L"Default_PS.hlsl", std::vector<RTexture>());
		mesh->Mat = defaultMaterial;
	}
	ObjectManager->AddGameObject(gameObject);

	gameObject->SetPosition(0, 0, 10);
}

void GameManager::Tick()
{

}