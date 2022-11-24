#include "TextureShader.h"

TextureShader::TextureShader()
{
}

TextureShader::TextureShader(const TextureShader&)
{
}

TextureShader::~TextureShader()
{
}

bool TextureShader::Init(ID3D11Device* Device, HWND Hwnd)
{
	wchar_t VsFilename[128];
	wchar_t PsFilename[128];

	int Error;

	Error = wcscpy_s(VsFilename, 128, L"D:/source/cpp/directx/Engine/Shaders/texture.vs");

	if (Error != 0) {
		return false;
	}

	Error = wcscpy_s(PsFilename, 128, L"D:/source/cpp/directx/Engine/Shaders/texture.ps");

	if (Error != 0) {
		return false;
	}

	bool bResult = InitShader(Device, Hwnd, VsFilename, PsFilename);

	if (!bResult) {
		return false;
	}

	return true;
}

void TextureShader::Shutdown()
{
	ShutdownShader();
}

bool TextureShader::Render(ID3D11DeviceContext* Context, int IndexCount, XMMATRIX WorldMatrix, XMMATRIX ViewMatrix, XMMATRIX ProjectionMatrix, ID3D11ShaderResourceView* Texture)
{
	bool bResult = SetShaderParameters(Context, WorldMatrix, ViewMatrix, ProjectionMatrix, Texture);

	if (!bResult) {
		return false;
	}

	RenderShader(Context, IndexCount);
	return true;
}

bool TextureShader::InitShader(ID3D11Device* Device, HWND Hwnd, WCHAR* VsFilename, WCHAR* PsFilename)
{
	HRESULT Result;
	ID3D10Blob* ErrorMessage = nullptr;
	ID3D10Blob* VertexShaderBuffer = nullptr;
	ID3D10Blob* PixelShaderBuffer = nullptr;
	D3D11_INPUT_ELEMENT_DESC PolygonLayout[2];
	unsigned int NumElements;
	D3D11_BUFFER_DESC MatrixBufferDesc;

	D3D11_SAMPLER_DESC SamplerDesc;

	Result = D3DCompileFromFile(VsFilename, NULL, NULL, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&VertexShaderBuffer, &ErrorMessage);

	if (FAILED(Result)) {
		if (ErrorMessage) {
			OutputShaderErrorMessage(ErrorMessage, Hwnd, VsFilename);
		}
		else {
			MessageBox(Hwnd, VsFilename, L"Missing Vertex Shader File", MB_OK);
		}
		return false;
	}

	Result = D3DCompileFromFile(PsFilename, NULL, NULL, "TexturePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&PixelShaderBuffer, &ErrorMessage);

	if (FAILED(Result)) {
		if (ErrorMessage) {
			OutputShaderErrorMessage(ErrorMessage, Hwnd, PsFilename);
		}
		else {
			MessageBox(Hwnd, PsFilename, L"Missing Pixel Shader File", MB_OK);
		}
		return false;
	}

	Result = Device->CreateVertexShader(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), NULL, &VertexShader);

	if (FAILED(Result)) {
		return false;
	}

	Result = Device->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), NULL, &PixelShader);

	if (FAILED(Result)) {
		return false;
	}

	PolygonLayout[0].SemanticName = "POSITION";
	PolygonLayout[0].SemanticIndex = 0;
	PolygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	PolygonLayout[0].InputSlot = 0;
	PolygonLayout[0].AlignedByteOffset = 0;
	PolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[0].InstanceDataStepRate = 0;

	PolygonLayout[1].SemanticName = "TEXCOORD";
	PolygonLayout[1].SemanticIndex = 0;
	PolygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	PolygonLayout[1].InputSlot = 0;
	PolygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	PolygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[1].InstanceDataStepRate = 0;

	NumElements = sizeof(PolygonLayout) / sizeof(PolygonLayout[0]);

	Result = Device->CreateInputLayout(PolygonLayout, NumElements, VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), &Layout);

	if (FAILED(Result)) {
		return false;
	}

	VertexShaderBuffer->Release();
	VertexShaderBuffer = nullptr;

	PixelShaderBuffer->Release();
	VertexShaderBuffer = nullptr;

	MatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	MatrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	MatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	MatrixBufferDesc.MiscFlags = 0;
	MatrixBufferDesc.StructureByteStride = 0;

	Result = Device->CreateBuffer(&MatrixBufferDesc, NULL, &MatrixBuffer);

	if (FAILED(Result)) {
		return false;
	}

	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerDesc.MipLODBias = 0.f;
	SamplerDesc.MaxAnisotropy = 1;
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	SamplerDesc.BorderColor[0] = 0;
	SamplerDesc.BorderColor[1] = 0;
	SamplerDesc.BorderColor[2] = 0;
	SamplerDesc.BorderColor[3] = 0;
	SamplerDesc.MinLOD = 0;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	Result = Device->CreateSamplerState(&SamplerDesc, &SampleState);

	if (FAILED(Result)) {
		return false;
	}

	return true;
}

void TextureShader::ShutdownShader()
{
	if (SampleState) {
		SampleState->Release();
		SampleState = nullptr;
	}

	if (MatrixBuffer) {
		MatrixBuffer->Release();
		MatrixBuffer = nullptr;
	}

	if (Layout) {
		Layout->Release();
		MatrixBuffer = nullptr;
	}

	if (PixelShader) {
		PixelShader->Release();
		PixelShader = nullptr;
	}

	if (VertexShader) {
		VertexShader->Release();
		VertexShader = nullptr;
	}
}

void TextureShader::OutputShaderErrorMessage(ID3D10Blob* ErrorMessage, HWND Hwnd, WCHAR* ShaderFilename)
{
	char* CompileErrors;
	unsigned long long BufferSize, i;
	std::ofstream Fout;

	CompileErrors = (char*)(ErrorMessage->GetBufferPointer());
	BufferSize = ErrorMessage->GetBufferSize();
	Fout.open("shader-error.txt");

	for (i = 0; i < BufferSize; i++) {
		Fout << CompileErrors[i];
	}

	Fout.close();

	ErrorMessage->Release();
	ErrorMessage = nullptr;

	MessageBox(Hwnd, L"Error compiling shader. Check shader-error.txt for message.", ShaderFilename, MB_OK);
}

bool TextureShader::SetShaderParameters(ID3D11DeviceContext* DeviceContext, XMMATRIX WorldMatrix, XMMATRIX ViewMatrix, XMMATRIX ProjectionMatrix, ID3D11ShaderResourceView* Texture)
{
	HRESULT Result;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	MatrixBufferType* DataPtr;
	unsigned int BufferNumber;

	WorldMatrix = XMMatrixTranspose(WorldMatrix);
	ViewMatrix = XMMatrixTranspose(ViewMatrix);
	ProjectionMatrix = XMMatrixTranspose(ProjectionMatrix);

	Result = DeviceContext->Map(MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);

	if (FAILED(Result)) {
		return false;
	}

	DataPtr = (MatrixBufferType*)MappedResource.pData;
	DataPtr->World = WorldMatrix;
	DataPtr->View = ViewMatrix;
	DataPtr->Projection = ProjectionMatrix;

	DeviceContext->Unmap(MatrixBuffer, 0);

	BufferNumber = 0;
	DeviceContext->VSSetConstantBuffers(BufferNumber, 1, &MatrixBuffer);

	DeviceContext->PSSetShaderResources(0, 1, &Texture);
	return true;
}

void TextureShader::RenderShader(ID3D11DeviceContext* DeviceContext, int IndexCount)
{
	DeviceContext->IASetInputLayout(Layout);

	DeviceContext->VSSetShader(VertexShader, NULL, 0);
	DeviceContext->PSSetShader(PixelShader, NULL, 0);

	DeviceContext->PSSetSamplers(0, 1, &SampleState);

	DeviceContext->DrawIndexed(IndexCount, 0, 0);
}
