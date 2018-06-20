#include "GameObject.h"
#include "Mesh.h"
#include "Material.h"


GameObject::~GameObject()
{
}

void GameObject::Start()
{

}

void GameObject::Tick()
{
}

const DirectX::XMMATRIX& GameObject::GetWorldMatrix()
{
	XMVECTOR positionVector = XMLoadFloat3(&m_position);

	XMMATRIX worldMatrix = DirectX::XMMatrixTranslationFromVector(positionVector);

	return worldMatrix;
}
