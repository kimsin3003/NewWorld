#include "RCamera.h"
#include <d3d11.h>
#include <DirectXMath.h>

RCamera::RCamera()
{
}


RCamera::~RCamera()
{
}

void RCamera::SetPosition(float x, float y, float z)
{
	m_position.x = x; m_position.y = y; m_position.z = z;
	CreateViewMatrix();
}

void RCamera::SetPosition(RVector3 pos)
{
	m_position = pos.ToXMFloat();
	CreateViewMatrix();
}

void RCamera::SetRotation(float x, float y, float z)
{
	m_rotation.x = x; m_rotation.y = y; m_rotation.z = z;
	CreateViewMatrix();
}

void RCamera::Initialize(float screenWidth, float screenHeight, float  screenNear, float screenDepth)
{
	m_position.x = 0;
	m_position.y = 0;
	m_position.z = 0;

	m_lookAt.x = 0;
	m_lookAt.y = 0;
	m_lookAt.z = 1;

	m_up.x = 0;
	m_up.y = 1;
	m_up.z = 0;

	m_rotation.x = 0;
	m_rotation.y = 0;
	m_rotation.z = 0;

	CreateViewMatrix();
	CreateProjectionMatrix(screenWidth, screenHeight, screenNear, screenDepth);
}

void RCamera::CreateProjectionMatrix(float screenWidth, float screenHeight, float screenNear, float screenDepth)
{
	float fieldOfView = 3.141592654f / 4.0f;
	float screenAspect = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering.
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
}

void RCamera::CreateViewMatrix()
{
	XMVECTOR positionVector = XMLoadFloat3(&m_position);
	XMVECTOR lookVector = XMLoadFloat3(&m_lookAt);
	XMVECTOR upVector = XMLoadFloat3(&m_up);
	
	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	float pitch = m_rotation.x * 0.0174532925f;
	float yaw = m_rotation.y * 0.0174532925f;
	float roll = m_rotation.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookVector = XMVector3Normalize(DirectX::XMVector3TransformCoord(lookVector, rotationMatrix));
	upVector = XMVector3Normalize(DirectX::XMVector3TransformCoord(upVector, rotationMatrix));

	// Translate the rotated camera position to the location of the viewer.
	XMVECTOR focusPos = DirectX::XMVectorAdd(positionVector, lookVector);
	XMStoreFloat3(&m_dir, lookVector);
	XMStoreFloat3(&m_right, XMVector3Normalize(XMVector3Cross(upVector, lookVector)));

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = DirectX::XMMatrixLookAtLH(positionVector, focusPos, upVector);
}
