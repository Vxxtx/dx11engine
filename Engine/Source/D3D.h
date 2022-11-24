#pragma once

#ifndef _D3D_H_
#define _D3D_H_

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

class D3D
{
public:
	D3D();
	D3D(const D3D&);
	~D3D();

	bool Init(int, int, bool, HWND, bool, float, float);
	void Shutdown();

	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);

	void GetVideoCardInfo(char*, int&);

private:
	bool VsyncEnabled;
	int VideoCardMemory;
	char VideoCardDesc[128];
	
	IDXGISwapChain* SwapChain;
	ID3D11Device* Device;
	ID3D11DeviceContext* DeviceContext;
	ID3D11RenderTargetView* RenderTargetView;
	ID3D11Texture2D* DepthStencilBuffer;
	ID3D11DepthStencilState* DepthStencilState;
	ID3D11DepthStencilView* DepthStencilView;
	ID3D11RasterizerState* RasterState;

	XMMATRIX ProjectionMatrix;
	XMMATRIX WorldMatrix;
	XMMATRIX OrthoMatrix;
};

#endif
