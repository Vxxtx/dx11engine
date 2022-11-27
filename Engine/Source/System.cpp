#include "System.h"

System::System()
{
}

System::System(const System& Other)
{
}

System::~System()
{
}

bool System::Init()
{
    int ScreenWidth, ScreenHeight;
    bool bResult;

    ScreenWidth = 0;
    ScreenHeight = 0;

    InitializeWindows(ScreenWidth, ScreenHeight);

    InputHandler = new Input;
    bResult = InputHandler->Init(HInstance, Hwnd, ScreenWidth, ScreenHeight);

    if (!bResult) {
        MessageBox(Hwnd, L"Could not initialize input component", L"Error", MB_OK);
        return false;
    }

    GraphicsHandler = new Graphics;
    bResult = GraphicsHandler->Init(ScreenWidth, ScreenHeight, Hwnd);

    if (!bResult) {
        return false;
    }

    PlayerCharacter = new Player(InputHandler, GraphicsHandler->GetCamera());
    bResult = PlayerCharacter->Init();

    if (!bResult) {
        MessageBox(Hwnd, L"Could not initialize player character", L"Error", MB_OK);
        return false;
    }

    return true;
}

void System::Shutdown()
{
    if (GraphicsHandler) {
        GraphicsHandler->Shutdown();
        delete GraphicsHandler;
        GraphicsHandler = nullptr;
    }

    if (InputHandler) {
        InputHandler->Shutdown();
        delete InputHandler;
        InputHandler = nullptr;
    }

    if (PlayerCharacter) {
        PlayerCharacter->Shutdown();
        delete PlayerCharacter;
        PlayerCharacter = nullptr;
    }

    ShutdownWindows();
}

void System::Run()
{
    MSG Msg;
    bool bIsDone, bResult;

    ZeroMemory(&Msg, sizeof(MSG));

    bIsDone = false;

    while (!bIsDone) {
        if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }

        if (Msg.message == WM_QUIT) {
            bIsDone = true;
        }
        else {
            bResult = Frame();

            if (!bResult) {
                bIsDone = true;
            }
        }

        if (InputHandler->IsEscapePressed()) {
            bIsDone = true;
        }
    }
}

LRESULT CALLBACK System::MessageHandler(HWND Hwnd, UINT Umsg, WPARAM Wparam, LPARAM Lparam)
{
    return DefWindowProc(Hwnd, Umsg, Wparam, Lparam);
}

bool System::Frame()
{
    bool bResult;
    int MouseX, MouseY;

    bResult = InputHandler->Frame();

    if (!bResult) {
        return false;
    }

    InputHandler->GetLastMouseLocation(MouseX, MouseY);

    bResult = GraphicsHandler->Frame(MouseX, MouseY);

    if (!bResult) {
        return false;
    }

    bResult = PlayerCharacter->Frame();

    if (!bResult) {
        return false;
    }

    return true;
}

void System::InitializeWindows(int& ScreenWidth, int& ScreenHeight)
{
    WNDCLASSEX Wc;
    DEVMODE DmScreenSettings;
    int PosX, PosY;

    ApplicationHandle = this;
    HInstance = GetModuleHandle(NULL);

    ApplicationName = L"Engine";

    Wc.style =          CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    Wc.lpfnWndProc =    WndProc;
    Wc.cbClsExtra =     0;
    Wc.cbWndExtra =     0;
    Wc.hInstance =      HInstance;
    Wc.hIcon =          LoadIcon(NULL, IDI_WINLOGO);
    Wc.hIconSm =        Wc.hIcon;
    Wc.hCursor =        LoadCursor(NULL, IDC_ARROW);
    Wc.hbrBackground =  (HBRUSH)GetStockObject(BLACK_BRUSH);
    Wc.lpszMenuName =   NULL;
    Wc.lpszClassName =  ApplicationName;
    Wc.cbSize =         sizeof(WNDCLASSEX);

    RegisterClassEx(&Wc);
    
    ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

    if (FULLSCREEN) {
        memset(&DmScreenSettings, 0, sizeof(DmScreenSettings));
        DmScreenSettings.dmSize = sizeof(DmScreenSettings);
        DmScreenSettings.dmPelsWidth = (unsigned long)ScreenWidth;
        DmScreenSettings.dmPelsHeight = (unsigned long)ScreenHeight;
        DmScreenSettings.dmBitsPerPel = 32;
        DmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        ChangeDisplaySettings(&DmScreenSettings, CDS_FULLSCREEN);

        PosX = PosY = 0;
    }
    else {
        ScreenWidth = 800;
        ScreenHeight = 600;

        PosX = (GetSystemMetrics(SM_CXSCREEN) - ScreenWidth) / 2;
        PosY = (GetSystemMetrics(SM_CYSCREEN) - ScreenHeight) / 2;
    }

    Hwnd = CreateWindowEx
    (
        WS_EX_APPWINDOW,
        ApplicationName, ApplicationName,
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
        PosX, PosY,
        ScreenWidth, ScreenHeight,
        NULL,
        NULL,
        HInstance,
        NULL
    );

    ShowWindow(Hwnd, SW_SHOW);
    SetForegroundWindow(Hwnd);
    SetFocus(Hwnd);

    ShowCursor(false);
}

void System::ShutdownWindows()
{
    ShowCursor(true);

    if (FULLSCREEN) {
        ChangeDisplaySettings(NULL, 0);
    }

    DestroyWindow(Hwnd);
    Hwnd = NULL;

    UnregisterClass(ApplicationName, HInstance);
    HInstance = NULL;

    ApplicationHandle = NULL;
}

LRESULT WndProc(HWND Hwnd, UINT Umessage, WPARAM Wparam, LPARAM Lparam)
{
    switch (Umessage) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
        
        default:
            return ApplicationHandle->MessageHandler(Hwnd, Umessage, Wparam, Lparam);
    }
}
