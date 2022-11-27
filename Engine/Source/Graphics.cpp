#include "Graphics.h"
#include <iostream>
#include <windows.h>
#include <string>

Graphics::Graphics()
{
}

Graphics::Graphics(const Graphics&)
{
}

Graphics::~Graphics()
{
}

bool Graphics::Init(int ScreenWidth, int ScreenHeight, HWND Hwnd)
{
	char ModelFilename[128];
	char TextureFilename[128];
	bool bResult;

	Direct3D = new D3D;

	bResult = Direct3D->Init(ScreenWidth, ScreenHeight, VSYNC_ENABLED, Hwnd, FULLSCREEN, SCREEN_DEPTH, SCREEN_NEAR);

	if (!bResult) {
		MessageBox(Hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	G_Camera = new Camera;
	G_Camera->SetPosition(0.f, 0.f, -135.f);

	G_Model = new Model;

	strcpy_s(ModelFilename, "D:/source/cpp/dx11engine/Engine/Data/Models/rpg.txt");
	strcpy_s(TextureFilename, "D:/source/cpp/dx11engine/Engine/Data/Textures/matti.tga");
	bResult = G_Model->Init(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), ModelFilename, TextureFilename);
	
	if (!bResult) {
		MessageBox(Hwnd, L"Could not initialize the model object", L"Error", MB_OK);
		return false;
	}

	G_LightShader = new LightShader;

	bResult = G_LightShader->Init(Direct3D->GetDevice(), Hwnd);

	if (!bResult) {
		MessageBox(Hwnd, L"Could not initialize the light shader object", L"Error", MB_OK);
		return false;
	}

	G_Light = new Light;

	if (!G_Light) {
		return false;
	}

	G_Light->SetDiffuseColor(1.f, 1.f, 1.f, 1.f);
	G_Light->SetDirection(0.f, 0.f, 1.f);

	return true;
}

void Graphics::Shutdown()
{
	if (G_Light) {
		delete G_Light;
		G_Light = nullptr;
	}

	if (G_LightShader) {
		G_LightShader->Shutdown();
		delete G_LightShader;
		G_LightShader = nullptr;
	}

	if (G_Model) {
		G_Model->Shutdown();
		delete G_Model;
		G_Model = nullptr;
	}

	if (G_Camera) {
		delete G_Camera;
		G_Camera = nullptr;
	}

	if (Direct3D) {
		Direct3D->Shutdown();
		delete Direct3D;
		Direct3D = nullptr;
	}
}

bool Graphics::Frame(int LastMouseX, int LastMouseY)
{
	bool Result = Render();

	if (!Result) {
		return false;
	}

	return true;
}

bool Graphics::Render()
{
	Direct3D->BeginScene(0.5f, 0.5f, 0.5f, 1.f);

	XMMATRIX WorldMatrix, ViewMatrix, ProjectionMatrix;

	Direct3D->BeginScene(0.f, 0.f, 0.f, 1.f);

	G_Camera->Render();

	Direct3D->GetWorldMatrix(WorldMatrix);
	G_Camera->GetViewMatrix(ViewMatrix);
	Direct3D->GetProjectionMatrix(ProjectionMatrix);

	//WorldMatrix = XMMatrixRotationY(Rotation);

	G_Model->Render(Direct3D->GetDeviceContext());

	bool bResult = G_LightShader->Render(Direct3D->GetDeviceContext(), G_Model->GetIndexCount(), WorldMatrix, ViewMatrix, ProjectionMatrix, G_Model->GetTexture(),
		G_Light->GetDirection(), G_Light->GetDiffuseColor());

	if (!bResult) {
		return false;
	}

	Direct3D->EndScene();
	return true;
}
