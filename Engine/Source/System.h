#pragma once

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include "Input.h"
#include "Graphics.h"

class System
{
public:
	System();
	System(const System&);
	~System();

	bool Init();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

	LPCWSTR ApplicationName;
	HINSTANCE HInstance;
	HWND Hwnd;

	Input* InputHandler;
	Graphics* GraphicsHandler;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static System* ApplicationHandle = 0;

#endif

