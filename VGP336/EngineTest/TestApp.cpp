#include "TestApp.h"

TestApp::TestApp()
: mMouseX(-1)
, mMouseY(-1)
{
	memset(mKeyStates, 0, sizeof(bool) * 256);
}

TestApp::~TestApp()
{
}

void TestApp::OnInitialize(u32 width, u32 height)
{
	// Init the window
	mWindow.Initialize(GetInstance(), GetAppName(), width, height);
	HookWindow(mWindow.GetWindowHandle());

	mGraphicsSystem.Initialize(mWindow.GetWindowHandle(), false);
	mCamera.Setup(Math::kPiByTwo, (f32)width / (f32)height, 0.01f, 10000.0f);
	mCamera.SetPosition(Math::Vector3(0.0f, 0.0f, -10.0f));

	mTimer.Initialize();

	SimpleDraw::Initialize(mGraphicsSystem);
}

void TestApp::OnTerminate()
{
	SimpleDraw::Terminate();
	mGraphicsSystem.Terminate();
	mWindow.Terminate();
}

bool TestApp::OnInput(const InputEvent& evt)
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

void TestApp::OnUpdate()
{
	if (mWindow.HandleMessage())
	{
		mRunning = false;
	}
	else
	{
		mTimer.Update();
		const f32 deltatime = mTimer.GetElapsedTime();
		const f32 kMoveSpeed = 10.0f;

		if (mKeyStates['W'])
			mCamera.Walk(kMoveSpeed * deltatime);
		else if (mKeyStates['A'])
			mCamera.Strafe(-kMoveSpeed * deltatime);
		else if (mKeyStates['D'])
			mCamera.Strafe(kMoveSpeed * deltatime);
		else if (mKeyStates['S'])
			mCamera.Walk(-kMoveSpeed * deltatime);

		mGraphicsSystem.BeginRender();

		SimpleDraw::AddLine(Math::Vector3::Zero(), Math::Vector3::XAxis(), Color::Red());
		SimpleDraw::AddLine(Math::Vector3::Zero(), Math::Vector3::YAxis(), Color::Green());
		SimpleDraw::AddLine(Math::Vector3::Zero(), Math::Vector3::ZAxis(), Color::Blue());

		SimpleDraw::Render(mCamera);
		mGraphicsSystem.EndRender();
	}
}