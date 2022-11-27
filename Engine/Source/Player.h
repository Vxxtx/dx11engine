#pragma once

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Element.h"
#include "Input.h"
#include "Camera.h"

#define DEBUG true

class Player : Element
{
public:
	Player(Input* InInput, Camera* InCamera)
		: Element() 
	{
		InputHandler = InInput;
		PlayerCamera = InCamera;
	};

	~Player();

public:
	inline Input* GetInput() { return InputHandler; };
	inline Camera* GetCamera() { return PlayerCamera; };

	virtual bool Init() override;
	virtual void Shutdown();
	
	virtual bool Frame() override;

private:
	Input* InputHandler{ nullptr };
	Camera* PlayerCamera{ nullptr };
};

#endif
