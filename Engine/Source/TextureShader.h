#pragma once

#ifndef TEXTURESHADER_H_
#define _TEXTURESHADER_H_

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>

using namespace DirectX;

class TextureShader
{
private:
	struct MatrixBufferType {
		XMMATRIX World;
		XMMATRIX View;
		XMMATRIX Projection;
	};
	
public:
	TextureShader();
	TextureShader(const TextureShader&);
	~TextureShader();

	bool Init(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);

private:
	bool InitShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);
	void RenderShader(ID3D11DeviceContext*, int);

	ID3D11VertexShader* VertexShader{ nullptr };
	ID3D11PixelShader* PixelShader{ nullptr };
	ID3D11InputLayout* Layout{ nullptr };
	ID3D11Buffer* MatrixBuffer{ nullptr };
	ID3D11SamplerState* SampleState{ nullptr };
};

#endif