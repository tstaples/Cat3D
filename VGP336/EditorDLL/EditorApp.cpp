#include "EditorApp.h"

EditorApp::EditorApp()
{
    memset(mKeyStates, 0, sizeof(bool) * 256);
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
        if(mMouseX != -1 && mMouseY != -1)
        {
            f32 deltaX = (f32)(evt.x - mMouseX);
            f32 deltaY = (f32)(evt.y - mMouseY);
            mCamera.Yaw(deltaX);
            mCamera.Pitch(deltaY);
        }
        mMouseX = evt.x;
        mMouseY = evt.y;
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

	// Camera movement
	f32 mouseSensitivity = 0.25f;
	mCamera.Yaw((f32)(mInputSystem.GetMouseMoveX()) * mouseSensitivity);
	mCamera.Pitch((f32)(mInputSystem.GetMouseMoveY()) * mouseSensitivity);

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
	mGraphicsSystem.BeginRender(Color::Red());

	//SimpleDraw::AddLine(Math::Vector3::Zero(), Math::Vector3::XAxis(), Color::Red());
	//SimpleDraw::AddLine(Math::Vector3::Zero(), Math::Vector3::YAxis(), Color::Green());
	//SimpleDraw::AddLine(Math::Vector3::Zero(), Math::Vector3::ZAxis(), Color::Blue());

	SimpleDraw::AddSphere(Math::Vector3(-2.0f, 0.0f, 0.0f), 2.0f, Color::White());
	SimpleDraw::AddSphere(Math::Vector3(2.0f, 0.0f, 0.0f), 2.0f, Color::White());

	SimpleDraw::Render(mCamera);
	mGraphicsSystem.EndRender();
}