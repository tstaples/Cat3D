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

void Application::HookWindow(HWND hWnd)
{
	SetWindowLongPtrA(hWnd, GWL_USERDATA, (LONG)this);
	SetWindowLong(hWnd, GWL_WNDPROC, (LONG)WndProc);

	mWindow = hWnd;
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

//====================================================================================================
// Window Message Procedure
//====================================================================================================

void MakeInputEvent(InputEvent& inputEvent, InputEvent::Type type, u32 value, LPARAM lParam)
{
    inputEvent.type = type;
    inputEvent.value = value;
	inputEvent.x = GET_X_LPARAM(lParam);
	inputEvent.y = GET_Y_LPARAM(lParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	InputEvent inputEvent;

	switch (message)
	{
	case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;
	case WM_CHAR:
		{
			inputEvent.type = InputEvent::Character;
			inputEvent.value = wParam;
		}
		break;
	case WM_KEYDOWN:
		{
			inputEvent.type = InputEvent::KeyDown;
			inputEvent.value = wParam;
		}
		break;
	case WM_KEYUP:
		{
			inputEvent.type = InputEvent::KeyUp;
			inputEvent.value = wParam;
		}
		break;
	case WM_MOUSEMOVE:
		{
			inputEvent.type = InputEvent::MouseMove;
			inputEvent.x = GET_X_LPARAM(lParam);
			inputEvent.y = GET_Y_LPARAM(lParam);
		}
		break;
        // TODO: rest of mouse button cases
    case WM_LBUTTONDOWN:
		{
			inputEvent.type = InputEvent::MouseDown;
            inputEvent.value = Mouse::LBUTTON;
			inputEvent.x = GET_X_LPARAM(lParam);
			inputEvent.y = GET_Y_LPARAM(lParam);
		}
		break;
    case WM_RBUTTONDOWN:
        {
			inputEvent.type = InputEvent::MouseDown;
            inputEvent.value = Mouse::RBUTTON;
			inputEvent.x = GET_X_LPARAM(lParam);
			inputEvent.y = GET_Y_LPARAM(lParam);
        }
        break;
    case WM_MBUTTONDOWN:
        {
			inputEvent.type = InputEvent::MouseDown;
            inputEvent.value = Mouse::MBUTTON;
			inputEvent.x = GET_X_LPARAM(lParam);
			inputEvent.y = GET_Y_LPARAM(lParam);
        }
        break;
    case WM_LBUTTONUP:
		{
			inputEvent.type = InputEvent::MouseUp;
            inputEvent.value = Mouse::LBUTTON;
			inputEvent.x = GET_X_LPARAM(lParam);
			inputEvent.y = GET_Y_LPARAM(lParam);
		}
		break;
    case WM_RBUTTONUP:
        {
			inputEvent.type = InputEvent::MouseUp;
            inputEvent.value = Mouse::RBUTTON;
			inputEvent.x = GET_X_LPARAM(lParam);
			inputEvent.y = GET_Y_LPARAM(lParam);
        }
        break;
    case WM_MBUTTONUP:
        {
			inputEvent.type = InputEvent::MouseUp;
            inputEvent.value = Mouse::MBUTTON;
			inputEvent.x = GET_X_LPARAM(lParam);
			inputEvent.y = GET_Y_LPARAM(lParam);
        }
        break;
    case WM_MOUSEWHEEL:
        {
            inputEvent.type = InputEvent::MouseScroll;
            inputEvent.value = Mouse::SCROLL;
            // Hack: storing wheel delta in y
            inputEvent.x = 0;
            inputEvent.y = GET_WHEEL_DELTA_WPARAM(wParam);
        }
        break;
	default:
		return DefWindowProcA(hWnd, message, wParam, lParam);
	}

    Application* myApp = (Application*)GetWindowLongPtrA(hWnd, GWL_USERDATA);
	if (myApp != nullptr)
	{
        if (!myApp->OnInput(inputEvent))
		{
			return DefWindowProcA(hWnd, message, wParam, lParam);
		}
    }
	return 0;
}