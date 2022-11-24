#include "D3D.h"

D3D::D3D()
{
	SwapChain = 0;
	Device = 0;
	DeviceContext = 0;
	RenderTargetView = 0;
	DepthStencilBuffer = 0;
	DepthStencilState = 0;
	DepthStencilView = 0;
	RasterState = 0;
}

D3D::D3D(const D3D&)
{
}

D3D::~D3D()
{
}

bool D3D::Init(int ScreenWidth, int ScreenHeight, bool Vsync, HWND Hwnd, 
	bool Fullscreen, float ScreenDepth, float ScreenNear)
{
	HRESULT Result;
	IDXGIFactory* Factory;
	IDXGIAdapter* Adapter;
	IDXGIOutput* AdapterOutput;
	unsigned int NumModes, i, Numerator, Denominator;
	unsigned long long StringLength;
	DXGI_MODE_DESC* DisplayModeList;
	DXGI_ADAPTER_DESC AdapterDesc;
	int Error;
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	D3D_FEATURE_LEVEL FeatureLevel;
	ID3D11Texture2D* BackBufferPtr;
	D3D11_TEXTURE2D_DESC DepthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
	D3D11_RASTERIZER_DESC RasterDesc;
	D3D11_VIEWPORT Viewport;
	float FieldOfView, ScreenAspect;

	VsyncEnabled = Vsync;
	
	Result = CreateDXGIFactory(_uuidof(IDXGIFactory), (void**)&Factory);

	if (FAILED(Result)) {
		return false;
	}

	Result = Factory->EnumAdapters(0, &Adapter);

	if (FAILED(Result)) {
		return false;
	}

	Result = Adapter->EnumOutputs(0, &AdapterOutput);

	if (FAILED(Result)) {
		return false;
	}

	Result = AdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 
		DXGI_ENUM_MODES_INTERLACED, &NumModes, NULL);

	if (FAILED(Result)) {
		return false;
	}

	DisplayModeList = new DXGI_MODE_DESC[NumModes];

	if (!DisplayModeList) {
		return false;
	}

	Result = AdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 
		DXGI_ENUM_MODES_INTERLACED, &NumModes, DisplayModeList);

	if (FAILED(Result)) {
		return false;
	}

	for (i = 0; i < NumModes; i++) {
		if (DisplayModeList[i].Width == (unsigned int)ScreenWidth) {
			if (DisplayModeList[i].Height == (unsigned int)ScreenHeight) {
				Numerator = DisplayModeList[i].RefreshRate.Numerator;
				Denominator = DisplayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	Result = Adapter->GetDesc(&AdapterDesc);

	if (FAILED(Result)) {
		return false;
	}

	VideoCardMemory = (int)(AdapterDesc.DedicatedVideoMemory / 1024 / 1024);

	Error = wcstombs_s(&StringLength, VideoCardDesc, 128, AdapterDesc.Description, 128);

	if (Error != 0) {
		return false;
	}

	delete[] DisplayModeList;
	DisplayModeList = 0;

	AdapterOutput->Release();
	AdapterOutput = 0;

	Adapter->Release();
	Adapter = 0;

	Factory->Release();
	Factory = 0;

	ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));

	SwapChainDesc.BufferCount = 1;

	SwapChainDesc.BufferDesc.Width = ScreenWidth;
	SwapChainDesc.BufferDesc.Height = ScreenHeight;

	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (VsyncEnabled) {
		SwapChainDesc.BufferDesc.RefreshRate.Numerator = Numerator;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = Denominator;
	}
	else {
		SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.OutputWindow = Hwnd;

	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;

	SwapChainDesc.Windowed = !Fullscreen;

	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	SwapChainDesc.Flags = 0;

	// Set feature level to DirectX 11
	FeatureLevel = D3D_FEATURE_LEVEL_11_0;

	Result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &FeatureLevel,
		1, D3D11_SDK_VERSION, &SwapChainDesc, &SwapChain, &Device, NULL, &DeviceContext);

	if (FAILED(Result)) {
		return false;
	}

	Result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBufferPtr);

	if (FAILED(Result)) {
		return false;
	}

	Result = Device->CreateRenderTargetView(BackBufferPtr, NULL, &RenderTargetView);

	if (FAILED(Result)) {
		return false;
	}

	BackBufferPtr->Release();
	BackBufferPtr = 0;

	ZeroMemory(&DepthBufferDesc, sizeof(DepthBufferDesc));

	DepthBufferDesc.Width = ScreenWidth;
	DepthBufferDesc.Height = ScreenHeight;
	DepthBufferDesc.MipLevels = 1;
	DepthBufferDesc.ArraySize = 1;
	DepthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthBufferDesc.SampleDesc.Count = 1;
	DepthBufferDesc.SampleDesc.Quality = 0;
	DepthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthBufferDesc.CPUAccessFlags = 0;
	DepthBufferDesc.MiscFlags = 0;

	Result = Device->CreateTexture2D(&DepthBufferDesc, NULL, &DepthStencilBuffer);

	if (FAILED(Result)) {
		return false;
	}

	ZeroMemory(&DepthStencilDesc, sizeof(DepthStencilDesc));

	DepthStencilDesc.DepthEnable = true;
	DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	DepthStencilDesc.StencilEnable = true;
	DepthStencilDesc.StencilReadMask = 0xFF;
	DepthStencilDesc.StencilWriteMask = 0xFF;

	DepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	DepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	DepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	DepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	Result = Device->CreateDepthStencilState(&DepthStencilDesc, &DepthStencilState);

	if (FAILED(Result)) {
		return false;
	}

	DeviceContext->OMSetDepthStencilState(DepthStencilState, 1);

	ZeroMemory(&DepthStencilViewDesc, sizeof(DepthStencilViewDesc));

	DepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDesc.Texture2D.MipSlice = 0;

	Result = Device->CreateDepthStencilView(DepthStencilBuffer, &DepthStencilViewDesc, &DepthStencilView);

	if (FAILED(Result)) {
		return false;
	}

	DeviceContext->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);

	RasterDesc.AntialiasedLineEnable = false;
	RasterDesc.CullMode = D3D11_CULL_BACK;
	RasterDesc.DepthBias = 0;
	RasterDesc.DepthBiasClamp = 0.f;
	RasterDesc.DepthClipEnable = true;
	RasterDesc.FillMode = D3D11_FILL_SOLID;
	RasterDesc.FrontCounterClockwise = false;
	RasterDesc.MultisampleEnable = false;
	RasterDesc.ScissorEnable = false;
	RasterDesc.SlopeScaledDepthBias = 0.f;

	Result = Device->CreateRasterizerState(&RasterDesc, &RasterState);

	if (FAILED(Result)) {
		return false;
	}

	DeviceContext->RSSetState(RasterState);

	Viewport.Width = (float)ScreenWidth;
	Viewport.Height = (float)ScreenHeight;
	Viewport.MinDepth = 0.f;
	Viewport.MaxDepth = 1.f;
	Viewport.TopLeftX = 0.f;
	Viewport.TopLeftY = 0.f;

	DeviceContext->RSSetViewports(1, &Viewport);
	
	FieldOfView = 3.141592654f / 4.f;
	ScreenAspect = (float)ScreenWidth / (float)ScreenHeight;

	ProjectionMatrix = XMMatrixPerspectiveFovLH(FieldOfView, ScreenAspect, ScreenNear, ScreenDepth);

	WorldMatrix = XMMatrixIdentity();

	OrthoMatrix = XMMatrixOrthographicLH((float)ScreenWidth, (float)ScreenHeight, ScreenNear, ScreenDepth);

	return true;
}

void D3D::Shutdown()
{
	if (SwapChain) {
		SwapChain->SetFullscreenState(false, NULL);
	}

	if (RasterState) {
		RasterState->Release();
		RasterState = 0;
	}

	if (DepthStencilView) {
		DepthStencilView->Release();
		DepthStencilView = 0;
	}

	if (DepthStencilState) {
		DepthStencilState->Release();
		DepthStencilState = 0;
	}

	if (DepthStencilBuffer) {
		DepthStencilBuffer->Release();
		DepthStencilBuffer = 0;
	}

	if (RenderTargetView) {
		RenderTargetView->Release();
		RenderTargetView = 0;
	}

	if (DeviceContext) {
		DeviceContext->Release();
		DeviceContext = 0;
	}

	if (Device) {
		Device->Release();
		Device = 0;
	}

	if (SwapChain) {
		SwapChain->Release();
		SwapChain = 0;
	}
}

void D3D::BeginScene(float R, float G, float B, float A)
{
	float Color[4];

	Color[0] = R;
	Color[1] = G;
	Color[2] = B;
	Color[3] = A;

	DeviceContext->ClearRenderTargetView(RenderTargetView, Color);
	DeviceContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0);
}

void D3D::EndScene()
{
	if (VsyncEnabled) {
		SwapChain->Present(1, 0);
	}
	else {
		SwapChain->Present(0, 0);
	}
}

ID3D11Device* D3D::GetDevice()
{
	return Device;
}

ID3D11DeviceContext* D3D::GetDeviceContext()
{
	return DeviceContext;
}

void D3D::GetProjectionMatrix(XMMATRIX& InProjMatrix)
{
	InProjMatrix = ProjectionMatrix;
}

void D3D::GetWorldMatrix(XMMATRIX& InWorldMatrix)
{
	InWorldMatrix = WorldMatrix;
}

void D3D::GetOrthoMatrix(XMMATRIX& InOrthoMatrix)
{
	InOrthoMatrix = OrthoMatrix;
}

void D3D::GetVideoCardInfo(char* CardName, int& Memory)
{
	strcpy_s(CardName, 128, VideoCardDesc);
	Memory = VideoCardMemory;
}
