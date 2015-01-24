#include "TestApp.h"

TestApp::TestApp()
{
}

TestApp::~TestApp()
{
}

void TestApp::OnInitialize(u32 width, u32 height)
{
	// Init the window
	mWindow.Initialize(GetInstance(), GetAppName(), width, height);
	HookWindow(mWindow.GetWindowHandle());

	mInputSystem.Initialize(mWindow.GetWindowHandle());
	mGraphicsSystem.Initialize(mWindow.GetWindowHandle(), false);
	SimpleDraw::Initialize(mGraphicsSystem);

	mCamera.Setup(Math::kPiByTwo, (f32)width / (f32)height, 0.01f, 10000.0f);
	mCamera.SetPosition(Math::Vector3(0.0f, 0.0f, -10.0f));
	mTimer.Initialize();

    //mTexture.Initialize(mGraphicsSystem, L"../Engine/Images/metalpanel.dds");
    //mTexture.AddRef();

	mRenderer.Initialize(mGraphicsSystem);
    //mRenderer.SetTexture(mTexture);

    //AssetLoader::LoadModel("../Data/Models/duck.catm", mGraphicsSystem, mModel);
    AssetLoader::LoadModel("../Data/Stuff/soldier.catm", mGraphicsSystem, mModel);
}

void TestApp::OnTerminate()
{
	mWindow.Terminate();
	mGraphicsSystem.Terminate();
	SimpleDraw::Terminate();
	mInputSystem.Terminate();
	
    //mTexture.Terminate();
    //mTexture.Release();
    
    mRenderer.Terminate();

    mModel.Unload();
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
	mGraphicsSystem.BeginRender();

	mRenderer.SetCamera(mCamera);
    mModel.Render(mRenderer);

	mGraphicsSystem.EndRender();
}