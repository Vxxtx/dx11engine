#include "Input.h"

Input::Input()
{
}

Input::Input(const Input&)
{
}

Input::~Input()
{
}

bool Input::Init(HINSTANCE HInstance, HWND Hwnd, int InScreenWidth, int InScreenHeight)
{
	HRESULT Result;

	ScreenWidth = InScreenWidth;
	ScreenHeight = InScreenHeight;

	MouseX = 0;
	MouseY = 0;

	Result = DirectInput8Create(HInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DirectInput, NULL);

	if (FAILED(Result)) {
		return false;
	}

	Result = DirectInput->CreateDevice(GUID_SysKeyboard, &Keyboard, NULL);

	if (FAILED(Result)) {
		return false;
	}

	Result = Keyboard->SetDataFormat(&c_dfDIKeyboard);

	if (FAILED(Result)) {
		return false;
	}

	Result = Keyboard->SetCooperativeLevel(Hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);

	if (FAILED(Result)) {
		return false;
	}

	Result = Keyboard->Acquire();

	if (FAILED(Result)) {
		return false;
	}

	Result = DirectInput->CreateDevice(GUID_SysMouse, &Mouse, NULL);

	if (FAILED(Result)) {
		return false;
	}

	Result = Mouse->SetDataFormat(&c_dfDIMouse);

	if (FAILED(Result)) {
		return false;
	}

	Result = Mouse->SetCooperativeLevel(Hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);

	if (FAILED(Result)) {
		return false;
	}

	return true;
	return false;
}

void Input::Shutdown()
{
	if (Mouse) {
		Mouse->Unacquire();
		Mouse->Release();
		Mouse = nullptr;
	}

	if (Keyboard) {
		Keyboard->Unacquire();
		Keyboard->Release();
		Keyboard = nullptr;
	}

	if (DirectInput) {
		DirectInput->Release();
		DirectInput = nullptr;
	}
}

bool Input::Frame()
{
	bool Result = ReadKeyboard();

	if (!Result) {
		return false;
	}

	Result = ReadMouse();

	if (!Result) {
		return false;
	}

	ProcessInput();
	return true;
}

bool Input::ReadKeyboard()
{
	HRESULT Result = Keyboard->GetDeviceState(sizeof(KeyboardState), (LPVOID)&KeyboardState);

	if (FAILED(Result)) {
		if (Result == DIERR_INPUTLOST || Result == DIERR_NOTACQUIRED) {
			Keyboard->Acquire();
		}
		else {
			return false;
		}
	}

	return true;
}

bool Input::ReadMouse()
{
	HRESULT Result;

	Result = Mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&MouseState);

	if (FAILED(Result)) {
		if (Result == DIERR_INPUTLOST || Result == DIERR_NOTACQUIRED) {
			Mouse->Acquire();
		}
		else {
			return false;
		}
	}

	return true;
}

void Input::ProcessInput()
{
	MouseX += MouseState.lX;
	MouseY += MouseState.lY;



	if (MouseX < 0) MouseX = 0;
	if (MouseY < 0) MouseY = 0;

	if (MouseX > ScreenWidth) MouseX = ScreenWidth;
	if (MouseY > ScreenHeight) MouseY = ScreenHeight;
}

bool Input::IsEscapePressed()
{
	if (KeyboardState[DIK_ESCAPE] & 0x80) {
		return true;
	}

	return false;
}

void Input::GetMouseLocation(int& OutMouseX, int& OutMouseY)
{
	OutMouseX = MouseX;
	OutMouseY = MouseY;
}
