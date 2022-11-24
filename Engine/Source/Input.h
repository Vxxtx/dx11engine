#pragma once

#ifndef _INPUT_H_
#define _INPUT_H_

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

class Input
{
public:
	Input();
	Input(const Input&);
	~Input();

	bool Init(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	void GetMouseLocation(int& MouseX, int& MouseY);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

	IDirectInput8* DirectInput{ nullptr };
	IDirectInputDevice8* Keyboard{ nullptr };
	IDirectInputDevice8* Mouse{ nullptr };

	unsigned char KeyboardState[256];
	DIMOUSESTATE MouseState;

	int ScreenWidth, ScreenHeight;
	int MouseX, MouseY;
};

#endif