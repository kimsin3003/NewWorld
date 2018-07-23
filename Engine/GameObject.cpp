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

const DirectX::XMMATRIX GameObject::GetWorldMatrix()
{
	XMVECTOR positionVector = XMLoadFloat3(&m_position);
	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	float pitch = m_rotation.x * 0.0174532925f;
	float yaw = m_rotation.y * 0.0174532925f;
	float roll = m_rotation.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	XMMATRIX positionMatrix = DirectX::XMMatrixTranslationFromVector(positionVector);

	XMMATRIX worldMatrix = DirectX::XMMatrixMultiply(rotationMatrix, positionMatrix);

	return worldMatrix;
}
