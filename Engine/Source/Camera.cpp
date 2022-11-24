#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(const Camera&)
{
}

Camera::~Camera()
{
}

void Camera::SetPosition(float X, float Y, float Z)
{
	PositionX = X;
	PositionY = Y;
	PositionZ = Z;
}

void Camera::SetRotation(float X, float Y, float Z)
{
	RotationX = X;
	RotationY = Y;
	RotationZ = Z;
}

XMFLOAT3 Camera::GetPosition()
{
	return XMFLOAT3(PositionX, PositionY, PositionZ);
}

XMFLOAT3 Camera::GetRotation()
{
	return XMFLOAT3(RotationX, RotationY, RotationZ);
}

void Camera::Render()
{
	XMFLOAT3 Up, Position, LookAt;
	XMVECTOR UpVector, PositionVector, LookAtVector;
	float Yaw, Pitch, Roll;
	XMMATRIX RotationMatrix;

	Up.x = 0.f;
	Up.y = 1.f;
	Up.z = 0.f;

	UpVector = XMLoadFloat3(&Up);

	Position.x = PositionX;
	Position.y = PositionY;
	Position.z = PositionZ;

	PositionVector = XMLoadFloat3(&Position);

	LookAt.x = 0.f;
	LookAt.y = 0.f;
	LookAt.z = 1.f;

	LookAtVector = XMLoadFloat3(&LookAt);

	Pitch = RotationX * 0.0174532925f;
	Yaw = RotationX * 0.0174532925f;
	Roll = RotationX * 0.0174532925f;

	RotationMatrix = XMMatrixRotationRollPitchYaw(Pitch, Yaw, Roll);

	LookAtVector = XMVector3TransformCoord(LookAtVector, RotationMatrix);
	UpVector = XMVector3TransformCoord(UpVector, RotationMatrix);

	LookAtVector = XMVectorAdd(PositionVector, LookAtVector);

	ViewMatrix = XMMatrixLookAtLH(PositionVector, LookAtVector, UpVector);
}

void Camera::GetViewMatrix(XMMATRIX& OutViewMatrix)
{
	OutViewMatrix = ViewMatrix;
}
