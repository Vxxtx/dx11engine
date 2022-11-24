#pragma once

#ifndef _LIGHTSHADER_H_
#define _LIGHTSHADER_H_

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>

using namespace DirectX;

class LightShader
{
private:
	struct MatrixBufferType {
		XMMATRIX World;
		XMMATRIX View;
		XMMATRIX Projection;
	};

	struct LightBufferType {
		XMFLOAT4 DiffuseColor;
		XMFLOAT3 LightDirection;
		float Padding;
	};

public:
	LightShader();
	LightShader(const LightShader&);
	~LightShader();

	bool Init(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4);

private:
	bool InitShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4);
	void RenderShader(ID3D11DeviceContext*, int);

	ID3D11VertexShader* VertexShader { nullptr };
	ID3D11PixelShader* PixelShader { nullptr };
	ID3D11InputLayout* Layout { nullptr };
	ID3D11Buffer* MatrixBuffer { nullptr };
	ID3D11Buffer* LightBuffer { nullptr };
};

#endif