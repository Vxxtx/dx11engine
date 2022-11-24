// Engine.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "System.h"
#include "Engine.h"

int WINAPI WinMain(HINSTANCE HInstance, HINSTANCE HPrevInstance, PSTR Pscmdline, int Icmdshow) {
	System* NewSystem;
	bool bResult;

	NewSystem = new System;
	bResult = NewSystem->Init();
	
	if (bResult) {
		NewSystem->Run();
	}

	NewSystem->Shutdown();
	delete NewSystem;
	NewSystem = 0;

	return 0;
}