#include "RGameObject.h"
#include "RMesh.h"
#include "RMaterial.h"
#include "RResourceLoader.h"

RGameObject::~RGameObject()
{
}

const DirectX::XMMATRIX RGameObject::GetWorldMatrix()
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

void RGameObject::SetResource(std::string filename)
{
	RResourceLoader::LoadFile(filename, this);
}
