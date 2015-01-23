#include "TestApp.h"

TestApp::TestApp()
{
}

TestApp::~TestApp()
{
}

void TestQuaternion()
{
    using namespace Math;

    f32 angle = kPi / 6.0f; // 60 degrees

    Quaternion q(Vector3::ZAxis(), angle);
    Quaternion qn = Normalize(q);

    Vector3 v(0.0f, 1.5f, 0.0f);
    Vector3 rot = RotateVector(v, qn);

    Matrix mZ = Matrix::RotationZ(angle);
    Matrix qm = Convert(q);
    ASSERT(qm == mZ, "Incorrect result: Converting quaternion to matrix");

    int i=0;
}


void TestApp::OnInitialize(u32 width, u32 height)
{
	// Init the window
	mWindow.Initialize(GetInstance(), GetAppName(), width, height);
	HookWindow(mWindow.GetWindowHandle());

	mTimer.Initialize();
	mInputSystem.Initialize(mWindow.GetWindowHandle());

	mGraphicsSystem.Initialize(mWindow.GetWindowHandle(), false);
	SimpleDraw::Initialize(mGraphicsSystem);

	mCamera.Setup(Math::kPiByTwo, (f32)width / (f32)height, 0.01f, 10000.0f);
	mCamera.SetPosition(Math::Vector3(0.0f, 0.0f, -10.0f));

    TestQuaternion();
}

void TestApp::OnTerminate()
{
	mInputSystem.Terminate();
	SimpleDraw::Terminate();
	mGraphicsSystem.Terminate();
	mWindow.Terminate();
}

bool TestApp::OnInput(const InputEvent& evt)
{
	bool handled = false;
	if (mInputSystem.IsKeyPressed(Keys::ESCAPE))
	{
		PostQuitMessage(0);
		handled = true;
	}
	return handled;
}

void TestApp::OnUpdate()
{
	if (mWindow.HandleMessage())
	{
		mRunning = false;
		return;
	}

	mTimer.Update();
	mInputSystem.Update();

	const f32 deltatime = mTimer.GetElapsedTime();
	const f32 kMoveSpeed = 10.0f;

	// Camera movement
	f32 mouseSensitivity = 0.25f;
	mCamera.Yaw((f32)(mInputSystem.GetMouseMoveX()) * mouseSensitivity);
	mCamera.Pitch((f32)(mInputSystem.GetMouseMoveY()) * mouseSensitivity);

	// Player movement
	if (mInputSystem.IsKeyDown(Keys::W))
		mCamera.Walk(kMoveSpeed * deltatime);
	else if (mInputSystem.IsKeyDown(Keys::A))
		mCamera.Strafe(-kMoveSpeed * deltatime);
	else if (mInputSystem.IsKeyDown(Keys::D))
		mCamera.Strafe(kMoveSpeed * deltatime);
	else if (mInputSystem.IsKeyDown(Keys::S))
		mCamera.Walk(-kMoveSpeed * deltatime);

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