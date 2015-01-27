#include "GameApp.h"

GameApp::GameApp()
    : mGameObjectRepo(Meta::GameObjectType, (u16)1)
    , mTransformRepo(Meta::TransformComponentType, (u16)1)
    , mModelRepo(Meta::ModelComponentType, (u16)1)
{
}

GameApp::~GameApp()
{
}

void GameApp::OnInitialize(u32 width, u32 height)
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

    mAssetLoader.Initialize(mGraphicsSystem);





    mID = mGameObjectRepo.Allocate();
    GameObject& gameObject = mGameObjectRepo.GetItem(mID);
    gameObject.mID = mID;
    
    ID transformId = mTransformRepo.Allocate();
    gameObject.AddComponent(transformId);

    mAssetLoader.LoadModel("../Data/Stuff/soldier.catm", mModel);

    ID modelID = mModelRepo.Allocate();
    ModelComponent& modelComponent = mModelRepo.GetItem(modelID);
    modelComponent.SetModel(&mModel);
    gameObject.AddComponent(modelID);

    mRenderService.Initialize(mGraphicsSystem, mGameObjectRepo, mTransformRepo, mModelRepo, mCamera);
    mRenderService.Subscribe(mID);
}

void GameApp::OnTerminate()
{
	mWindow.Terminate();
	mGraphicsSystem.Terminate();
	SimpleDraw::Terminate();
	mInputSystem.Terminate();
    //mRenderer.Terminate();

    mModel.Unload();
    mAssetLoader.Terminate();
    mRenderService.Terminate();
}

bool GameApp::OnInput(const InputEvent& evt)
{
	bool handled = false;
	if (mInputSystem.IsKeyPressed(Keys::ESCAPE))
	{
		PostQuitMessage(0);
		handled = true;
	}
	return handled;
}

void GameApp::OnUpdate()
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

    mRenderService.Update();

	/*mRenderer.SetCamera(mCamera);
    mModel.Render(mRenderer, Math::Matrix::Identity());*/

	mGraphicsSystem.EndRender();
}