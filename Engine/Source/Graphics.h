#pragma once

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "D3D.h"
#include "Camera.h"
#include "Model.h"
#include "TextureShader.h"

#include "LightShader.h"
#include "Light.h"

const bool FULLSCREEN{ false };
const bool VSYNC_ENABLED{ true };
const float SCREEN_DEPTH{ 1000.f };
const float SCREEN_NEAR{ 0.1f };

class Graphics
{
public:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();

	bool Init(int, int, HWND);
	void Shutdown();
	bool Frame(int, int);

private:
	bool Render();

	D3D* Direct3D{ nullptr };
	Camera* G_Camera{ nullptr };
	Model* G_Model{ nullptr };

	LightShader* G_LightShader{ nullptr };
	Light* G_Light{ nullptr };

public:
	inline Camera* GetCamera() { return G_Camera; };
};

#endif