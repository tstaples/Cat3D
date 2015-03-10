#include "EditorAPI.h"

#include "EditorApp.h"

namespace
{
    EditorApp app;
}

void Initialize(int* instancePtrAddress, int* hPrevInstancePtrAddress, int* hWndPtrAddress, int nCmdShow, int screenWidth, int screenHeight)
{
    // C# passes HINSTANCE and HWND values to C++ DLL as (int*)
    HINSTANCE hInstance = (HINSTANCE)instancePtrAddress;
    HINSTANCE hPrevInstance = (HINSTANCE)hPrevInstancePtrAddress;
    HWND hWnd = (HWND)hWndPtrAddress;

    app.HookWindow(hWnd);
    app.Initialize(hInstance, "Editor", screenWidth, screenHeight);
}

void WndProc(int* hWndPtrAddress, int msg, int wParam, int lParam)
{
}

void UpdateFrame()
{
    app.Update();
}

void Terminate()
{
    app.Terminate();
}

int IsGameRunning()
{
    return (int)app.IsRunning();
}