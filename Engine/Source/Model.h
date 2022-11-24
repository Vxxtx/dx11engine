#pragma once

#ifndef _MODEL_H_
#define _MODEL_H_

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

#include <fstream>

#include "Texture.h"

class Model
{
private:
	struct VertexType {
		XMFLOAT3 Position;
		XMFLOAT2 Texture;
		XMFLOAT3 Normal;
	};

	struct ModelType {
		float X, Y, Z;
		float U, V;
		float NX, NY, NZ;
	};

public:
	Model();
	Model(const Model&);
	~Model();

	bool Init(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();

	ID3D11Buffer* VertexBuffer{ nullptr };
	ID3D11Buffer* IndexBuffer{ nullptr };
	int VertexCount, IndexCount;

	Texture* Tex{ nullptr };

	ModelType* ModelData{ nullptr };
};

#endif