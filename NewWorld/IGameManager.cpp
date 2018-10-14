#include "Engine/IGameManager.h"
#include "Engine/RGameObject.h"
#include "Engine/RResourceLoader.h"
#include "Engine/RContext.h"
#include "Engine/RMaterial.h"
#include "Engine/RMesh.h"

void IGameManager::Initialize()
{
	RGameObject* gameObject = new RGameObject();
	
	gameObject->SetResource("rectangle.FBX");
	for (RMesh* mesh : gameObject->Meshes)
	{
		RMaterial* defaultMaterial = new RMaterial(L"Default_VS.hlsl", L"Default_PS.hlsl", std::vector<RTexture>());
		mesh->Mat = defaultMaterial;
	}
	ObjectManager->AddGameObject(gameObject);
}

void IGameManager::Tick()
{

}