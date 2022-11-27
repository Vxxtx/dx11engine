#pragma once

#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#include <DirectXMath.h>

class Element
{
public:
	Element();
	~Element();

public:
	virtual bool Init();
	virtual void Shutdown();

protected:
	virtual bool Frame();

private:
	DirectX::XMFLOAT3 Position {0.f, 0.f, -105.f};
	DirectX::XMFLOAT3 Rotation {0.f, 0.f, 0.f};

public:
	inline void SetPosition(DirectX::XMFLOAT3 InPos) { Position = InPos; };
	DirectX::XMFLOAT3 GetPosition() { return Position; };

	inline void SetRotation(DirectX::XMFLOAT3 InRot) { Rotation = InRot; };
	DirectX::XMFLOAT3 GetRotation() { return Rotation; };

	DirectX::XMFLOAT3 GetForward();
	DirectX::XMFLOAT3 GetRight();
	DirectX::XMFLOAT3 GetUp();
};

#endif