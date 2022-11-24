#pragma once

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <DirectXMath.h>
using namespace DirectX;

class Camera
{
public:
	Camera();
	Camera(const Camera&);
	~Camera();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix(XMMATRIX&);

private:
	float PositionX{ 0 }, PositionY{ 0 }, PositionZ{ 0 };
	float RotationX{ 0 }, RotationY{ 0 }, RotationZ{ 0 };
	XMMATRIX ViewMatrix;
};

#endif
