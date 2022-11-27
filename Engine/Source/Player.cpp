#include "Player.h"

Player::~Player()
{
}

bool Player::Init()
{
	Element::Init();

	return true;
}

void Player::Shutdown()
{
	Element::Shutdown();
}

bool Player::Frame()
{
	Element::Frame();

	// Camera rotation
	XMFLOAT3 Rot = GetRotation();
	int MX, MY;

	GetInput()->GetLastMouseLocation(MY, MX);

	float CamNewX = Rot.x + MX;
	float CamNewY = Rot.y + MY;

	if (CamNewX > 90.f) {
		CamNewX = 90.f;
	}

	if (CamNewX < -90.f) {
		CamNewX = -90.f;
	}

	if (CamNewY > 360.f || CamNewY < -360.f) {
		CamNewY = 0.f;
	}

	SetRotation(XMFLOAT3(CamNewX, CamNewY, 0.f));
	GetCamera()->SetRotation(CamNewX, CamNewY, 0.f);

	// Camera free movement

	if (InputHandler->IsKeyPressed(DIK_W)) {
		DirectX::XMFLOAT3 ForwardFloat = GetForward();
		DirectX::XMFLOAT3 Pos = GetPosition();

		XMVECTOR ForwardVec = XMLoadFloat3(&ForwardFloat);
		XMVECTOR PosVec = XMLoadFloat3(&Pos);

		XMFLOAT3 FinalPos;
		XMStoreFloat3(&FinalPos, ForwardVec + PosVec);

		SetPosition(FinalPos);
	}

	if (InputHandler->IsKeyPressed(DIK_A)) {
		DirectX::XMFLOAT3 Right = GetRight();
		DirectX::XMFLOAT3 Pos = GetPosition();

		XMVECTOR RightVec = XMLoadFloat3(&Right);
		XMVECTOR PosVec = XMLoadFloat3(&Pos);

		XMFLOAT3 FinalPos;
		XMStoreFloat3(&FinalPos, RightVec * -1 + PosVec);

		SetPosition(FinalPos);
	}

	if (InputHandler->IsKeyPressed(DIK_S)) {
		DirectX::XMFLOAT3 ForwardFloat = GetForward();
		DirectX::XMFLOAT3 Pos = GetPosition();

		XMVECTOR ForwardVec = XMLoadFloat3(&ForwardFloat);
		XMVECTOR PosVec = XMLoadFloat3(&Pos);

		XMFLOAT3 FinalPos;
		XMStoreFloat3(&FinalPos, ForwardVec * -1 + PosVec);

		SetPosition(FinalPos);
	}

	if (InputHandler->IsKeyPressed(DIK_D)) {
		DirectX::XMFLOAT3 Right = GetRight();
		DirectX::XMFLOAT3 Pos = GetPosition();

		XMVECTOR RightVec = XMLoadFloat3(&Right);
		XMVECTOR PosVec = XMLoadFloat3(&Pos);

		XMFLOAT3 FinalPos;
		XMStoreFloat3(&FinalPos, RightVec + PosVec);

		SetPosition(FinalPos);
	}

	DirectX::XMFLOAT3 Pos = GetPosition();

	if (InputHandler->IsKeyPressed(DIK_E)) {
		SetPosition(XMFLOAT3(Pos.x, Pos.y + 1.f, Pos.z));
	}

	if (InputHandler->IsKeyPressed(DIK_Q)) {
		SetPosition(XMFLOAT3(Pos.x, Pos.y - 1.f, Pos.z));
	}

	GetCamera()->SetPosition(GetPosition().x, GetPosition().y, GetPosition().z);

#if DEBUG
	wchar_t xbuffer[256];
	wsprintfW(xbuffer, L"%d", (int)CamNewX);

	wchar_t ybuffer[256];
	wsprintfW(ybuffer, L"%d", (int)CamNewY);

	OutputDebugString(L"MouseX: ");
	OutputDebugString(xbuffer);
	OutputDebugString(L"\n");

	OutputDebugString(L"MouseY: ");
	OutputDebugString(ybuffer);
	OutputDebugString(L"\n");
#endif

	return true;
}
