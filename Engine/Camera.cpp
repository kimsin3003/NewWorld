#include "Camera.h"
#include <d3d11.h>
#include <DirectXMath.h>

Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
	m_position.x = x; m_position.y = y; m_position.z = z;
	CreateViewMatrix();
}

void Camera::Initialize(float screenWidth, float screenHeight, float  screenNear, float screenDepth)
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

	CreateViewMatrix();
	CreateProjectionMatrix(screenWidth, screenHeight, screenNear, screenDepth);
}

void Camera::CreateProjectionMatrix(float screenWidth, float screenHeight, float screenNear, float screenDepth)
{
	float fieldOfView = 3.141592654f / 4.0f;
	float screenAspect = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering.
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
}

void Camera::CreateViewMatrix()
{
	XMVECTOR eyePosition = XMLoadFloat3(&m_position);
	XMVECTOR focusPosition = XMLoadFloat3(&m_lookAt);
	XMVECTOR upDirection = XMLoadFloat3(&m_up);
	
	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	float pitch = m_rotation.x * 0.0174532925f;
	float yaw = m_rotation.y * 0.0174532925f;
	float roll = m_rotation.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	focusPosition = XMVector3TransformCoord(focusPosition, rotationMatrix);
	upDirection = XMVector3TransformCoord(upDirection, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	eyePosition = XMVectorAdd(eyePosition, focusPosition);

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = DirectX::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
}