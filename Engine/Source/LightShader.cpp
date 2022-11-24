#include "LightShader.h"

LightShader::LightShader()
{
}

LightShader::LightShader(const LightShader&)
{
}

LightShader::~LightShader()
{
}

bool LightShader::Init(ID3D11Device* Device, HWND Hwnd)
{
	wchar_t VsFilename[128];
	wchar_t PsFilename[128];

	int Error;
	Error = wcscpy_s(VsFilename, 128, L"D:/source/cpp/directx/Engine/Shaders/light.vs");

	if (Error != 0) {
		return false;
	}

	Error = wcscpy_s(PsFilename, 128, L"D:/source/cpp/directx/Engine/Shaders/light.ps");

	if (Error != 0) {
		return false;
	}

	bool bResult = InitShader(Device, Hwnd, VsFilename, PsFilename);

	if (!bResult) {
		return false;
	}

	return true;
}

void LightShader::Shutdown()
{
	ShutdownShader();
}

bool LightShader::Render(ID3D11DeviceContext* DeviceContext, int IndexCount, XMMATRIX WorldMatrix, XMMATRIX ViewMatrix, XMMATRIX ProjectionMatrix,
	ID3D11ShaderResourceView* Texture, XMFLOAT3 LightDirection, XMFLOAT4 DiffuseColor)
{
	bool bResult = SetShaderParameters(DeviceContext, WorldMatrix, ViewMatrix, ProjectionMatrix, Texture, LightDirection, DiffuseColor);

	if (!bResult) {
		return false;
	}

	RenderShader(DeviceContext, IndexCount);
	return true;
}

bool LightShader::InitShader(ID3D11Device* Device, HWND Hwnd, WCHAR* VsFilename, WCHAR* PsFilename)
{
	HRESULT Result;
	ID3D10Blob* ErrorMessage = nullptr;
	ID3D10Blob* VertexShaderBuffer = nullptr;
	ID3D10Blob* PixelShaderBuffer = nullptr;
	D3D11_INPUT_ELEMENT_DESC PolygonLayout[3];
	unsigned int NumElements;
	D3D11_BUFFER_DESC MatrixBufferDesc;
	D3D11_BUFFER_DESC LightBufferDesc;

	Result = D3DCompileFromFile(VsFilename, NULL, NULL, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
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

	Result = D3DCompileFromFile(PsFilename, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
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

	PolygonLayout[2].SemanticName = "NORMAL";
	PolygonLayout[2].SemanticIndex = 0;
	PolygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	PolygonLayout[2].InputSlot = 0;
	PolygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	PolygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[2].InstanceDataStepRate = 0;

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

	LightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	LightBufferDesc.ByteWidth = sizeof(LightBufferType);
	LightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	LightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	LightBufferDesc.MiscFlags = 0;
	LightBufferDesc.StructureByteStride = 0;

	Result = Device->CreateBuffer(&LightBufferDesc, NULL, &LightBuffer);

	if (FAILED(Result)) {
		return false;
	}

	return true;
}

void LightShader::ShutdownShader()
{
	if (LightBuffer) {
		LightBuffer->Release();
		LightBuffer = nullptr;
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

void LightShader::OutputShaderErrorMessage(ID3D10Blob* ErrorMessage, HWND Hwnd, WCHAR* ShaderFilename)
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

bool LightShader::SetShaderParameters(ID3D11DeviceContext* DeviceContext, XMMATRIX WorldMatrix, XMMATRIX ViewMatrix, XMMATRIX ProjectionMatrix,
	ID3D11ShaderResourceView* Texture, XMFLOAT3 LightDirection, XMFLOAT4 DiffuseColor)
{
	HRESULT Result;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	MatrixBufferType* DataPtr = nullptr;
	LightBufferType* LightDataPtr = nullptr;
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

	Result = DeviceContext->Map(LightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);

	if (FAILED(Result)) {
		return false;
	}

	LightDataPtr = (LightBufferType*)MappedResource.pData;

	LightDataPtr->DiffuseColor = DiffuseColor;
	LightDataPtr->LightDirection = LightDirection;
	LightDataPtr->Padding = 0.f;

	DeviceContext->Unmap(LightBuffer, 0);
	BufferNumber = 0;

	DeviceContext->PSSetConstantBuffers(BufferNumber, 1, &LightBuffer);

	return true;
}

void LightShader::RenderShader(ID3D11DeviceContext* DeviceContext, int IndexCount)
{
	DeviceContext->IASetInputLayout(Layout);

	DeviceContext->VSSetShader(VertexShader, NULL, 0);
	DeviceContext->PSSetShader(PixelShader, NULL, 0);

	DeviceContext->DrawIndexed(IndexCount, 0, 0);
}
