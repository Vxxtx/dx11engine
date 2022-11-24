#include "Texture.h"

Texture::Texture()
{
}

Texture::Texture(const Texture&)
{
}

Texture::~Texture()
{
}

bool Texture::Init(ID3D11Device* Device, ID3D11DeviceContext* Context, char* Filename)
{
	int Width, Height;
	D3D11_TEXTURE2D_DESC TextureDesc;
	HRESULT Result;
	unsigned int RowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc;

	bool bResult = LoadTarga(Filename, Width, Height);

	if (!bResult) {
		return false;
	}

	TextureDesc.Width = Width;
	TextureDesc.Height = Height;
	TextureDesc.MipLevels = 0;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	Result = Device->CreateTexture2D(&TextureDesc, NULL, &Tex);

	if (FAILED(Result)) {
		return false;
	}

	RowPitch = (Width * 4) * sizeof(unsigned char);

	Context->UpdateSubresource(Tex, 0, NULL, TargaData, RowPitch, 0);

	SrvDesc.Format = TextureDesc.Format;
	SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SrvDesc.Texture2D.MostDetailedMip = 0;
	SrvDesc.Texture2D.MipLevels = -1;

	Result = Device->CreateShaderResourceView(Tex, &SrvDesc, &TexView);

	if (FAILED(Result)) {
		return false;
	}

	Context->GenerateMips(TexView);
	
	delete[] TargaData;
	TargaData = nullptr;
	
	return true;
}

void Texture::Shutdown()
{
	if (TexView) {
		TexView->Release();
		TexView = nullptr;
	}

	if (Tex) {
		Tex->Release();
		Tex = nullptr;
	}

	if (TargaData) {
		delete[] TargaData;
		TargaData = nullptr;
	}
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return TexView;
}

bool Texture::LoadTarga(char* Filename, int& Width, int& Height)
{
	int Error, Bpp, ImageSize, Index, i, j, k;

	FILE* FilePtr = nullptr;
	unsigned int Count = 0;
	TargaHeader TargaFileHeader {};
	unsigned char* TargaImage = nullptr;

	Error = fopen_s(&FilePtr, Filename, "rb");

	if (Error != 0) {
		return false;
	}

	Count = (unsigned int)fread(&TargaFileHeader, sizeof(TargaHeader), 1, FilePtr);

	if (Count != 1) {
		return false;
	}

	Width = (int)TargaFileHeader.Width;
	Height = (int)TargaFileHeader.Height;
	Bpp = (int)TargaFileHeader.Bpp;

	if (Bpp != 32) {
		return false;
	}

	ImageSize = Width * Height * 4;

	TargaImage = new unsigned char[ImageSize];

	Count = (unsigned int)fread(TargaImage, 1, ImageSize, FilePtr);

	if (Count != ImageSize) {
		return false;
	}

	Error = fclose(FilePtr);

	if (Error != 0) {
		return false;
	}

	TargaData = new unsigned char[ImageSize];

	Index = 0;

	k = (Width * Height * 4) - (Width * 4);

	for (j = 0; j < Height; j++) {
		for (i = 0; i < Width; i++) {
			TargaData[Index + 0] = TargaImage[k + 2]; // R
			TargaData[Index + 1] = TargaImage[k + 1]; // G
			TargaData[Index + 2] = TargaImage[k + 0]; // B
			TargaData[Index + 3] = TargaImage[k + 3]; // A

			k += 4;
			Index += 4;
		}

		k -= (Width * 8);
	}

	delete[] TargaImage;
	TargaImage = nullptr;
	
	return true;
}
