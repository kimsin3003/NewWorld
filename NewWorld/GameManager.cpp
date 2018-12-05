#include "GameManager.h"
#include "Engine/RMaterial.h"
#include "Engine/RMesh.h"
#include "Engine/RObjectManager.h"
#include "Room.h"
#include "Engine/RLight.h"
#include "Engine/PbrSphere.h"
#include "Engine/PbrPlane.h"

void GameManager::Initialize()
{
	for (int i = 0; i < 5; i++)
	{
		PbrSphere* sphere = new PbrSphere();
		sphere->R = 2 * i;
		sphere->SetPosition(-50 + i * 20, 0, 200);
		sphere->pbrColor = { 6, 189, 0 };
		sphere->pbrFigure = PBRFIGURE::SPHERE;
		ObjectManager->AddGameObject(sphere);
	}

	PbrPlane* center = new PbrPlane();
	center->V1 = DirectX::XMFLOAT3(-100, 50, 250);
	center->V2 = DirectX::XMFLOAT3(100, 50, 250);
	center->V3 = DirectX::XMFLOAT3(100, -50, 250);
	center->V4 = DirectX::XMFLOAT3(-100, -50, 250);
	center->pbrColor = { 0, 183, 255 };
	center->pbrFigure = PBRFIGURE::PLANE;
	ObjectManager->AddGameObject(center);

	PbrPlane* left = new PbrPlane();
	left->V1 = DirectX::XMFLOAT3(-100, 50, 150);
	left->V2 = DirectX::XMFLOAT3(-100, 50, 250);
	left->V3 = DirectX::XMFLOAT3(-100, -50, 250);
	left->V4 = DirectX::XMFLOAT3(-100, -50, 150);
	left->pbrColor = { 0, 183, 255 };
	left->pbrFigure = PBRFIGURE::PLANE;
	ObjectManager->AddGameObject(left);

	PbrPlane* right = new PbrPlane();
	right->V1 = DirectX::XMFLOAT3(100, 50, 250);
	right->V2 = DirectX::XMFLOAT3(100, 50, 150);
	right->V3 = DirectX::XMFLOAT3(100, -50, 150);
	right->V4 = DirectX::XMFLOAT3(100, -50, 250);
	right->pbrColor = { 0, 183, 255 };
	right->pbrFigure = PBRFIGURE::PLANE;
	ObjectManager->AddGameObject(right);

	PbrPlane* top = new PbrPlane();
	top->V1 = DirectX::XMFLOAT3(-100, 50, 150);
	top->V2 = DirectX::XMFLOAT3(100, 50, 150);
	top->V3 = DirectX::XMFLOAT3(100, 50, 250);
	top->V4 = DirectX::XMFLOAT3(-100, 50, 250);
	top->pbrColor = { 0, 183, 255 };
	top->pbrFigure = PBRFIGURE::PLANE;
	ObjectManager->AddGameObject(top);

	PbrPlane* bottom = new PbrPlane();
	bottom->V1 = DirectX::XMFLOAT3(100, -50, 150);
	bottom->V2 = DirectX::XMFLOAT3(-100, -50, 150);
	bottom->V3 = DirectX::XMFLOAT3(-100, -50, 250);
	bottom->V4 = DirectX::XMFLOAT3(100, -50, 250);
	bottom->pbrColor = { 0, 183, 255 };
	bottom->pbrFigure = PBRFIGURE::PLANE;
	ObjectManager->AddGameObject(bottom);
}

void GameManager::Tick(double deltaTime)
{
}