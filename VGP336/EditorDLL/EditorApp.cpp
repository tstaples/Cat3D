#include "EditorApp.h"

EditorApp::EditorApp()
    : mMouseX(0)
    , mMouseY(0)
    , mMouseMoveX(0)
    , mMouseMoveY(0)
    , mMouseScrollDelta(0.0f)
{
    memset(mKeyStates, 0, sizeof(bool) * 256);
    memset(mMouseStates, 0, sizeof(bool) * 4);
}

EditorApp::~EditorApp()
{
}


void EditorApp::OnInitialize(u32 width, u32 height)
{
	mTimer.Initialize();

	mGraphicsSystem.Initialize(GetWindow(), false);
	SimpleDraw::Initialize(mGraphicsSystem);

	mCamera.Setup(Math::kPiByTwo, (f32)width / (f32)height, 0.01f, 10000.0f);
	mCamera.SetPosition(Math::Vector3(0.0f, 0.0f, -10.0f));
}

void EditorApp::OnTerminate()
{
	SimpleDraw::Terminate();
	mGraphicsSystem.Terminate();
}

bool EditorApp::OnInput(const InputEvent& evt)
{
    bool handled = false;
    switch(evt.type)
    {
    case InputEvent::KeyDown:
        {
            if(evt.value == VK_ESCAPE)
            {
                PostQuitMessage(0);
            }
               
            mKeyStates[evt.value] = true;
            handled = true;
        }
        break;
    case InputEvent::KeyUp:
        {
            mKeyStates[evt.value] = false;
            handled = true;
        }
        break;
    case InputEvent::MouseMove:
        {
            if (mMouseX != -1 && mMouseY != -1)
            {
                // Get the offset since the last frame
                mMouseMoveX = (f32)(evt.x - mMouseX);
                mMouseMoveY = (f32)(evt.y - mMouseY);
            }
            mMouseX = evt.x;
            mMouseY = evt.y;
            handled = true;
        }
        break;
    case InputEvent::MouseDown:
        {
            mMouseStates[evt.value] = true;
            handled = true;
        }
        break;
    case InputEvent::MouseUp:
        {
            mMouseStates[evt.value] = false;
            handled = true;
        }
        break;
    case InputEvent::MouseScroll:
        {
            mMouseScrollDelta = evt.y;
            handled = true;
        }
        break;
    }
    return handled;
}

void EditorApp::OnUpdate()
{
	mTimer.Update();

	const f32 deltaTime = mTimer.GetElapsedTime();
	const f32 kMoveSpeed = 10.0f;

	// Camera movement modifiers (TODO: Make these configurable)
	const f32 lookSensitivity = 0.5f;
    const f32 moveSensitivity = 0.25f;

    // Camera look
    if (mMouseStates[Mouse::RBUTTON])
    {
        mCamera.Yaw(mMouseMoveX * lookSensitivity);
        mCamera.Pitch(mMouseMoveY * lookSensitivity);
    }
    // Move camera
    if (mMouseStates[Mouse::MBUTTON])
    {
        mCamera.Strafe((-mMouseMoveX) * moveSensitivity);
        mCamera.Rise(mMouseMoveY * moveSensitivity);
    }
    // Camera zoom (TODO)
    if (mMouseScrollDelta > 0)
    {
    }

    bool temp = false; // used for testing below statement
    if (mMouseStates[Mouse::LBUTTON])
    {
        // TODO: http://antongerdelan.net/opengl/raycasting.html
    }

	// Player movement
    if(mKeyStates['W'])
    {
        mCamera.Walk(kMoveSpeed * deltaTime);
    }
    else if(mKeyStates['S'])
    {
        mCamera.Walk(-kMoveSpeed * deltaTime);
    }
    else if(mKeyStates['D'])
    {
        mCamera.Strafe(kMoveSpeed * deltaTime);
    }
    else if(mKeyStates['A'])
    {
        mCamera.Strafe(-kMoveSpeed * deltaTime);
    }

	// Render
	mGraphicsSystem.BeginRender(Color::Black());

    Color sphereCol = (temp) ? Color::Red() : Color::White();
	SimpleDraw::AddSphere(Math::Vector3(-2.0f, 0.0f, 0.0f), 2.0f, sphereCol);
	SimpleDraw::AddSphere(Math::Vector3(2.0f, 0.0f, 0.0f), 2.0f, Color::White());

	SimpleDraw::Render(mCamera);
	mGraphicsSystem.EndRender();
}