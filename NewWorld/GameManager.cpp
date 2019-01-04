#include "GameManager.h"
#include "Engine/RMaterial.h"
#include "Engine/RMesh.h"
#include "Engine/RObjectManager.h"
#include "Room.h"
#include "Engine/PbrSphere.h"
#include "Engine/PbrPlane.h"
#include "Engine/UIObject.h"

void GameManager::Initialize()
{
	for (int i = 0; i < 3; i++)
	{
		PbrSphere* sphere = new PbrSphere();
		sphere->R = 2;
		sphere->SetPosition(-5 + i * 5, -2, 30);
		sphere->pbrColor = { 0.1, 0.1, 0.1 };
		sphere->pbrFigure = PBRFIGURE::SPHERE;
		sphere->pbrGlossy = i != 1 ? true : false;
		ObjectManager->AddGameObject(sphere);
	}

	PbrSphere* light = new PbrSphere();
	light->R = 1;
	light->SetPosition(3, 2, 20);
	light->pbrColor = { 1, 1, 1 };
	light->pbrFigure = PBRFIGURE::SPHERE;
	light->IsLight = true;
	ObjectManager->AddGameObject(light);

	PbrPlane* center = new PbrPlane();
	center->V1 = DirectX::XMFLOAT3(-10, 10, 50);
	center->V2 = DirectX::XMFLOAT3(10, 10, 50);
	center->V3 = DirectX::XMFLOAT3(10, -10, 50);
	center->V4 = DirectX::XMFLOAT3(-10, -10, 50);
	center->pbrColor = { 0.3, 0.3, 0.3};
	center->pbrFigure = PBRFIGURE::PLANE;
	center->pbrGlossy = true;
	ObjectManager->AddGameObject(center);

	PbrPlane* left = new PbrPlane();
	left->V1 = DirectX::XMFLOAT3(-10, 10, 0);
	left->V2 = DirectX::XMFLOAT3(-10, 10, 50);
	left->V3 = DirectX::XMFLOAT3(-10, -10, 50);
	left->V4 = DirectX::XMFLOAT3(-10, -10, 0);
	left->pbrColor = { 0.4, 0, 0.52 };
	left->pbrFigure = PBRFIGURE::PLANE;
	ObjectManager->AddGameObject(left);

	PbrPlane* right = new PbrPlane();
	right->V1 = DirectX::XMFLOAT3(10, 10, 50);
	right->V2 = DirectX::XMFLOAT3(10, 10, 0);
	right->V3 = DirectX::XMFLOAT3(10, -10, 0);
	right->V4 = DirectX::XMFLOAT3(10, -10, 50);
	right->pbrColor = { 0.4, 0.52, 0 };
	right->pbrFigure = PBRFIGURE::PLANE;
	ObjectManager->AddGameObject(right);

	PbrPlane* top = new PbrPlane();
	top->V1 = DirectX::XMFLOAT3(-10, 10, 0);
	top->V2 = DirectX::XMFLOAT3(10, 10, 0);
	top->V3 = DirectX::XMFLOAT3(10, 10, 50);
	top->V4 = DirectX::XMFLOAT3(-10, 10, 50);
	top->pbrColor = { 0, 0.52 , 0.4 };
	top->pbrFigure = PBRFIGURE::PLANE;
	ObjectManager->AddGameObject(top);

	PbrPlane* bottom = new PbrPlane();
	bottom->V1 = DirectX::XMFLOAT3(10, -5, 0);
	bottom->V2 = DirectX::XMFLOAT3(-10, -5, 0);
	bottom->V3 = DirectX::XMFLOAT3(-10, -5, 50);
	bottom->V4 = DirectX::XMFLOAT3(10, -5, 50);
	bottom->pbrColor = { 0.52, 0, 0.4 };
	bottom->pbrFigure = PBRFIGURE::PLANE;
	ObjectManager->AddGameObject(bottom);

	RTexture texture;
	texture.filename = L"pbroutput.bmp";
	texture.type = RTexture::DIFFUSE;

	std::vector<RTexture> textures;    
	textures.push_back(texture);
	RMaterial* uiMaterial = new RMaterial(L"UI_VS.hlsl", L"UI_PS.hlsl", textures);

	UIObject* ui = new UIObject();
	RMesh* uiMesh = new RMesh();
	uiMesh->Mat = uiMaterial;
	RVertex leftTop;
	leftTop.UV = { 0,0 };
	leftTop.Pos = { -1, 1, 0 };
	uiMesh->Verticies.push_back(leftTop);
	RVertex rightTop;
	rightTop.UV = { 1,0 };
	rightTop.Pos = { 1, 1, 0 };
	uiMesh->Verticies.push_back(rightTop);
	RVertex rightBottom;
	rightBottom.UV = { 1,1 };
	rightBottom.Pos = { 1, -1, 0 };
	uiMesh->Verticies.push_back(rightBottom);
	RVertex leftBottom;
	leftBottom.UV = { 0,1 };
	leftBottom.Pos = { -1, -1, 0 };
	uiMesh->Verticies.push_back(leftBottom);

	uiMesh->Indicies = { 3, 0, 1, 2, 3,1 };

	ui->Meshes.push_back(uiMesh);

	ObjectManager->AddGameObject(ui);
}

void GameManager::Tick(double deltaTime)
{
}