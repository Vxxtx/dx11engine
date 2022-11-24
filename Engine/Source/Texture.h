#pragma once

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <d3d11.h>
#include <stdio.h>

class Texture
{
private:
	struct TargaHeader {
		unsigned char Data1[12];
		unsigned short Width;
		unsigned short Height;
		unsigned char Bpp;
		unsigned char Data2;
	};

public:
	Texture();
	Texture(const Texture&);
	~Texture();

	bool Init(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool LoadTarga(char*, int&, int&);

	unsigned char* TargaData{ nullptr };
	ID3D11Texture2D* Tex{ nullptr };
	ID3D11ShaderResourceView* TexView{ nullptr };
};

#endif