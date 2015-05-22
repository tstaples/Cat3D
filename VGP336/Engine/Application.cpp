//====================================================================================================
// Filename:	Application.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "Application.h"

#include "InputEvent.h"
#include "InputTypes.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

Application::Application()
	: mInstance(nullptr)
	, mWindow(nullptr)
	, mRunning(true)
    , mOwner(true)
{
}

//----------------------------------------------------------------------------------------------------

Application::~Application()
{
}

//----------------------------------------------------------------------------------------------------

void Application::Initialize(HINSTANCE instance, LPCSTR appName, u32 width, u32 height)
{
	mInstance = instance;
	mAppName = appName;

	OnInitialize(width, height);
}

//----------------------------------------------------------------------------------------------------

void Application::Terminate()
{
	OnTerminate();
}

//----------------------------------------------------------------------------------------------------

void Application::HookWindow(HWND hWnd, bool owner)
{
	SetWindowLongPtrA(hWnd, GWL_USERDATA, (LONG)this);
	SetWindowLong(hWnd, GWL_WNDPROC, (LONG)WndProc);

	mWindow = hWnd;
    mOwner = owner;
}

//----------------------------------------------------------------------------------------------------

void Application::UnhookWindow()
{
	SetWindowLongPtrA(mWindow, GWL_USERDATA, 0);
	SetWindowLong(mWindow, GWL_WNDPROC, (LONG)DefWindowProc);

	mWindow = nullptr;
}

//----------------------------------------------------------------------------------------------------

void Application::Update()
{
	OnUpdate();
}

//----------------------------------------------------------------------------------------------------

void Application::ForwardInput(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    InputEvent inputEvent;
    ProcessInput(hWnd, message, wParam, lParam, inputEvent);
    OnInput(inputEvent);
}

//----------------------------------------------------------------------------------------------------

bool Application::ProcessInput(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, InputEvent& inputEvent)
{
    bool handled = true;

    switch (message)
	{
    case WM_SIZE:
        OnResizeWindow();
        break;
	case WM_CHAR:
		{
			inputEvent.type = Input::Character;
			inputEvent.value = wParam;
		}
		break;
	case WM_KEYDOWN:
		{
			inputEvent.type = Input::KeyDown;
			inputEvent.value = wParam;
		}
		break;
	case WM_KEYUP:
		{
			inputEvent.type = Input::KeyUp;
			inputEvent.value = wParam;
		}
		break;
	case WM_MOUSEMOVE:
		{
			inputEvent.type = Input::MouseMove;
			inputEvent.x = GET_X_LPARAM(lParam);
			inputEvent.y = GET_Y_LPARAM(lParam);
		}
		break;
        // TODO: rest of mouse button cases
    case WM_LBUTTONDOWN:
		{
			inputEvent.type = Input::MouseDown;
            inputEvent.value = Mouse::LBUTTON;
			inputEvent.x = GET_X_LPARAM(lParam);
			inputEvent.y = GET_Y_LPARAM(lParam);
		}
		break;
    case WM_RBUTTONDOWN:
        {
			inputEvent.type = Input::MouseDown;
            inputEvent.value = Mouse::RBUTTON;
			inputEvent.x = GET_X_LPARAM(lParam);
			inputEvent.y = GET_Y_LPARAM(lParam);
        }
        break;
    case WM_MBUTTONDOWN:
        {
			inputEvent.type = Input::MouseDown;
            inputEvent.value = Mouse::MBUTTON;
			inputEvent.x = GET_X_LPARAM(lParam);
			inputEvent.y = GET_Y_LPARAM(lParam);
        }
        break;
    case WM_LBUTTONUP:
		{
			inputEvent.type = Input::MouseUp;
            inputEvent.value = Mouse::LBUTTON;
			inputEvent.x = GET_X_LPARAM(lParam);
			inputEvent.y = GET_Y_LPARAM(lParam);
		}
		break;
    case WM_RBUTTONUP:
        {
			inputEvent.type = Input::MouseUp;
            inputEvent.value = Mouse::RBUTTON;
			inputEvent.x = GET_X_LPARAM(lParam);
			inputEvent.y = GET_Y_LPARAM(lParam);
        }
        break;
    case WM_MBUTTONUP:
        {
			inputEvent.type = Input::MouseUp;
            inputEvent.value = Mouse::MBUTTON;
			inputEvent.x = GET_X_LPARAM(lParam);
			inputEvent.y = GET_Y_LPARAM(lParam);
        }
        break;
    case WM_MOUSEWHEEL:
        {
            inputEvent.type = Input::MouseScroll;
            inputEvent.value = Mouse::SCROLL;
            inputEvent.wheeldelta = (GET_WHEEL_DELTA_WPARAM(wParam) > 0) ? 1 : -1;
            inputEvent.x = GET_X_LPARAM(lParam);
			inputEvent.y = GET_Y_LPARAM(lParam);
        }
        break;
    default:
        handled = false;
        break;
	}
    return handled;
}

//====================================================================================================
// Window Message Procedure
//====================================================================================================

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
	HDC hdc;

    switch (message)
    {
    case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		break;
    }

    Application* myApp = (Application*)GetWindowLongPtrA(hWnd, GWL_USERDATA);
	if (myApp != nullptr)
	{
        // Only quit if we own the window instance
        if (message == WM_DESTROY && myApp->IsOwner())
        {
			PostQuitMessage(0);
        }

        InputEvent evt;
        if (myApp->ProcessInput(hWnd, message, wParam, lParam, evt))
        {
            if (myApp->OnInput(evt))
		    {
            	return 0;
		    }
        }
    }
	return DefWindowProcA(hWnd, message, wParam, lParam);
}