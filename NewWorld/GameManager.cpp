#include "GameManager.h"
#include "Engine/RMaterial.h"
#include "Engine/RMesh.h"
#include "Engine/RObjectManager.h"
#include "Room.h"
#include "Engine/PbrSphere.h"
#include "Engine/PbrPlane.h"
#include "Engine/UIObject.h"
#include "Engine/RCameraManager.h"

void GameManager::Initialize()
{
	CameraManager->GetCurrentCamera()->SetPosition(0, 15, 0);
	CameraManager->GetCurrentCamera()->SetRotation(15, 0, 0);

	PbrSphere* redSphere = new PbrSphere();
	redSphere->R = 3;
	redSphere->SetPosition(-5, 3, 40);
	redSphere->PbrColliderType = PBRColliderType::SPHERE;
	redSphere->PbrTransparent = false;
	redSphere->DiffuseRate = 0.7;
	redSphere->Reflectance = { 0.7, 0.0, 0.0 };
	ObjectManager->AddPbrObject(redSphere);

	PbrSphere* greenSphere = new PbrSphere();
	greenSphere->R = 3;
	greenSphere->SetPosition(0, 3, 45);
	greenSphere->PbrColliderType = PBRColliderType::SPHERE;
	greenSphere->PbrTransparent = false;
	greenSphere->DiffuseRate = 0.7;
	greenSphere->Reflectance = { 0.0, 0.7, 0.0 };
	ObjectManager->AddPbrObject(greenSphere);

	PbrSphere* blueSphere = new PbrSphere();
	blueSphere->R = 3;
	blueSphere->SetPosition(5, 3, 40);
	blueSphere->PbrColliderType = PBRColliderType::SPHERE;
	blueSphere->PbrTransparent = false;
	blueSphere->DiffuseRate = 0.7;
	blueSphere->Reflectance = { 0.0, 0.0, 0.7 };
	ObjectManager->AddPbrObject(blueSphere);

	PbrSphere* trasparent = new PbrSphere();
	trasparent->R = 3;
	trasparent->SetPosition(0, 5, 30);
	trasparent->PbrColliderType = PBRColliderType::SPHERE;
	trasparent->PbrTransparent = true;
	trasparent->RefractionRate = 1.5f;
	blueSphere->DiffuseRate = 0.7;
	trasparent->Reflectance = { 1, 1, 1 };
	ObjectManager->AddPbrObject(trasparent);

	PbrSphere* light = new PbrSphere();
	light->R = 1;
	light->SetPosition(10, 3, 30);
	light->IsLight = true;
	light->Emittance = { 2, 2, 2 };
	light->PbrColliderType = PBRColliderType::SPHERE;
	light->PbrTransparent = true;
	light->DiffuseRate = 0.7;
	light->Reflectance = { 1, 1, 1 };
	ObjectManager->AddPbrObject(light);

	PbrPlane* front = new PbrPlane();
	front->V1 = DirectX::XMFLOAT3(20, -20, 0); 
	front->V2 = DirectX::XMFLOAT3(-20, -20, 0); 
	front->V3 = DirectX::XMFLOAT3(-20, 20, 0);
	front->V4 = DirectX::XMFLOAT3(20, 20, 0);
	front->Reflectance = { 0.7, 0.7, 0.7 };
	front->PbrColliderType = PBRColliderType::PLANE;
	ObjectManager->AddPbrObject(front);

	PbrPlane* center = new PbrPlane();
	center->V1 = DirectX::XMFLOAT3(20, 20, 70);
	center->V2 = DirectX::XMFLOAT3(-20, 20, 70);
	center->V3 = DirectX::XMFLOAT3(-20, -20, 70);
	center->V4 = DirectX::XMFLOAT3(20, -20, 70);
	center->Reflectance = { 0.7, 0.7, 0.7 };
	center->PbrColliderType = PBRColliderType::PLANE;
	ObjectManager->AddPbrObject(center);

	PbrPlane* left = new PbrPlane();
	left->V1 = DirectX::XMFLOAT3(-20, 20, 70);
	left->V2 = DirectX::XMFLOAT3(-20, 20, 0);
	left->V3 = DirectX::XMFLOAT3(-20, -20, 0);
	left->V4 = DirectX::XMFLOAT3(-20, -20, 70);
	left->Reflectance = { 0.7, 0.7, 0.7 };
	left->PbrColliderType = PBRColliderType::PLANE;
	ObjectManager->AddPbrObject(left);

	PbrPlane* right = new PbrPlane();
	right->V1 = DirectX::XMFLOAT3(20, 20, 0);
	right->V2 = DirectX::XMFLOAT3(20, 20, 70);
	right->V3 = DirectX::XMFLOAT3(20, -20, 70);
	right->V4 = DirectX::XMFLOAT3(20, -20, 0);
	right->Reflectance = { 0.7, 0.7, 0.7 };
	right->PbrColliderType = PBRColliderType::PLANE;
	ObjectManager->AddPbrObject(right);

	PbrPlane* top = new PbrPlane();
	top->V1 = DirectX::XMFLOAT3(-20, 20, 0);
	top->V2 = DirectX::XMFLOAT3(-20, 20, 70);
	top->V3 = DirectX::XMFLOAT3(20, 20, 70);
	top->V4 = DirectX::XMFLOAT3(20, 20, 0);
	top->Reflectance = { 0.7, 0.7, 0.7 };
	top->PbrColliderType = PBRColliderType::PLANE;
	ObjectManager->AddPbrObject(top);

	PbrPlane* bottom = new PbrPlane();
	bottom->V1 = DirectX::XMFLOAT3(20, 0, 0);
	bottom->V2 = DirectX::XMFLOAT3(20, 0, 70);
	bottom->V3 = DirectX::XMFLOAT3(-20, 0, 70);
	bottom->V4 = DirectX::XMFLOAT3(-20, 0, 0);
	bottom->Reflectance = { 0.7, 0.7, 0.7 };
	bottom->PbrColliderType = PBRColliderType::PLANE;
	ObjectManager->AddPbrObject(bottom);

	RTexture texture;
	texture.filename = L"result.bmp";
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