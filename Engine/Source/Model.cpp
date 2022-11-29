#include "Model.h"

Model::Model()
{
}

Model::Model(const Model&)
{
}

Model::~Model()
{
}

bool Model::Init(ID3D11Device* Device, ID3D11DeviceContext* Context, char* ModelFilename, char* TextureFilename)
{
	bool bResult = LoadModel(ModelFilename);

	if (!bResult) {
		return false;
	}

	bResult = InitBuffers(Device);
	if (!bResult) {
		return false;
	}
	
	bResult = LoadTexture(Device, Context, TextureFilename);

	if (!bResult) {
		return false;
	}

	return true;
}

void Model::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
	ReleaseModel();
}

void Model::Render(ID3D11DeviceContext* DeviceContext)
{
	RenderBuffers(DeviceContext);
}

int Model::GetIndexCount()
{
	return IndexCount;
}

ID3D11ShaderResourceView* Model::GetTexture()
{
	return Tex->GetTexture();
}

bool Model::InitBuffers(ID3D11Device* Device)
{
	VertexType* Vertices;
	unsigned long* Indices;

	D3D11_BUFFER_DESC VertexBufferDesc, IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA VertexData, IndexData;
	HRESULT Result;
	
	Vertices = new VertexType[VertexCount];
	Indices = new unsigned long[IndexCount];

	// Model world position is also added here. 
	for (int i = 0; i < VertexCount; i++) {
		Vertices[i].Position = XMFLOAT3(ModelData[i].X + Position.x, ModelData[i].Y + Position.y, ModelData[i].Z + Position.z);
		Vertices[i].Texture = XMFLOAT2(ModelData[i].U, ModelData[i].V);
		Vertices[i].Normal = XMFLOAT3(ModelData[i].NX, ModelData[i].NY, ModelData[i].NZ);

		Indices[i] = i;
	}

	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth = sizeof(VertexType) * VertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	VertexData.pSysMem = Vertices;
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;

	Result = Device->CreateBuffer(&VertexBufferDesc, &VertexData, &VertexBuffer);

	if (FAILED(Result)) {
		return false;
	}

	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(unsigned long) * IndexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	IndexData.pSysMem = Indices;
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;

	Result = Device->CreateBuffer(&IndexBufferDesc, &IndexData, &IndexBuffer);

	if (FAILED(Result)) {
		return false;
	}

	delete[] Vertices;
	Vertices = nullptr;

	delete[] Indices;
	Indices = nullptr;

	return true;
}

void Model::ShutdownBuffers()
{
	if (IndexBuffer) {
		IndexBuffer->Release();
		IndexBuffer = nullptr;
	}

	if (VertexBuffer) {
		VertexBuffer->Release();
		VertexBuffer = nullptr;
	}
}

void Model::RenderBuffers(ID3D11DeviceContext* DeviceContext)
{
	unsigned int Stride = sizeof(VertexType);
	unsigned int Offset = 0;

	DeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &Stride, &Offset);
	DeviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Model::LoadTexture(ID3D11Device* Device, ID3D11DeviceContext* Context, char* Filename)
{
	Tex = new Texture;

	bool bResult = Tex->Init(Device, Context, Filename);

	if (!bResult) {
		return false;
	}

	return true;
}

void Model::ReleaseTexture()
{
	if (Tex) {
		Tex->Shutdown();
		delete Tex;
		Tex = nullptr;
	}
}

bool Model::LoadModel(char* Filename)
{
	std::ifstream FileIn;
	char Input;

	FileIn.open(Filename);

	if (FileIn.fail()) {
		return false;
	}

	FileIn.get(Input);
	while (Input != ':') {
		FileIn.get(Input);
	}

	FileIn >> VertexCount;
	IndexCount = VertexCount;

	ModelData = new ModelType[VertexCount];

	if (!ModelData) {
		return false;
	}

	FileIn.get(Input);
	while (Input != ':') {
		FileIn.get(Input);
	}
	FileIn.get(Input);
	FileIn.get(Input);

	for (int i = 0; i < VertexCount; i++) {
		FileIn >> ModelData[i].X >> ModelData[i].Y >> ModelData[i].Z;
		FileIn >> ModelData[i].U >> ModelData[i].V;
		FileIn >> ModelData[i].NX >> ModelData[i].NY >> ModelData[i].NZ;
	}

	FileIn.close();

	return true;
}

void Model::ReleaseModel()
{
	if (ModelData) {
		delete[] ModelData;
		ModelData = nullptr;
	}
}
