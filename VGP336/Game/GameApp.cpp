#include "GameApp.h"

/* TODO
Level data file
- Holds instance specific data
{
    "Level" : [
        "GameObjects" : [
            "Template file" : "../Data/soldier.tmpl",
            "Template file" : "../Data/urmum.tmpl",
        ],
        "Transforms" : [
            { "x" : "0", "y" : "0", "z" : "0" },
            { "x" : "0", "y" : "0", "z" : "0" }
        ]
    ]
*/

GameApp::GameApp()
    : mGameObjectRepo(Meta::GameObjectType, 1)
    , mTransformRepo(Meta::TransformComponentType, 1)
    , mModelRepo(Meta::ModelComponentType, 1)
    , mGameObjectFactory(mGameObjectRepo, 
                         mTransformRepo, 
                         mModelRepo, 
                         mRenderService,
                         mModelManager)
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

    // Subsystems
	mInputSystem.Initialize(mWindow.GetWindowHandle());
	mGraphicsSystem.Initialize(mWindow.GetWindowHandle(), false);
	SimpleDraw::Initialize(mGraphicsSystem);
	mTimer.Initialize();

    // Camera
    mCamera.Setup(Math::kPiByTwo, (f32)width / (f32)height, 0.01f, 10000.0f);
	mCamera.SetPosition(Math::Vector3(0.0f, 50.0f, 15.0f));
    mCamera.SetLookAt(Math::Vector3::Zero());

    // Asset managers
    mTextureManager.Initialize(mGraphicsSystem);
    mModelManager.Initialize(mGraphicsSystem);
    mRenderService.Initialize(mGraphicsSystem, mGameObjectRepo, mTransformRepo, mModelRepo, mCamera);

    // Terrain
    //mTerrain.Initialize(mGraphicsSystem, "../Data/Heightmaps/heightmap.raw", 256, 256, 30.0f);
    //mTerrain.SetCamera(mCamera);
    //mTerrain.SetLayer(mTextureManager.GetResource(L"../Data/Images/rock.jpg"), 0, 0.0f, 20.0f, 17.0f);
    //mTerrain.SetLayer(mTextureManager.GetResource(L"../Data/Images/grass.jpg"), 1, 10.0f, 30.0f, 21.0f);

    // Create the soldier model
    mGameObjectFactory.Create("../Data/GameObjects/groundplane.json", Math::Vector3::Zero());
    mBallPos = Math::Vector3::Zero();
}

void GameApp::OnTerminate()
{
    mTransformRepo.Flush();
    mModelRepo.Flush();
    mGameObjectRepo.Flush();

    //mTerrain.Terminate();

    mModelManager.Terminate();
    mTextureManager.Terminate();
    mRenderService.Terminate();

    SimpleDraw::Terminate();
	mGraphicsSystem.Terminate();
	mInputSystem.Terminate();
	mWindow.Terminate();
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
	const f32 kMoveSpeed = 100.0f;

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

    if (mInputSystem.IsKeyDown(Keys::UP))
		mBallPos.z += (kMoveSpeed * deltatime);
	else if (mInputSystem.IsKeyDown(Keys::DOWN))
		mBallPos.z += (-kMoveSpeed * deltatime);
	else if (mInputSystem.IsKeyDown(Keys::RIGHT))
		mBallPos.x += (kMoveSpeed * deltatime);
	else if (mInputSystem.IsKeyDown(Keys::LEFT))
		mBallPos.x += (-kMoveSpeed * deltatime);
    mBallPos.y = mTerrain.GetHeight(mBallPos);

	// Render
	mGraphicsSystem.BeginRender();

    //mTerrain.Render();
    mRenderService.Update();

    SimpleDraw::AddSphere(mBallPos, 10.0f, Color::Red());

    SimpleDraw::Render(mCamera);
	mGraphicsSystem.EndRender();
}

/*
class StackAllocator
{
public:
    template <typename T>
    T New()
    {
        u8* ptr = buffer + offset;
        new (ptr) T();
        offset += sizeof(T);
        return ptr;
    }

    template <typename T>
    T NewArray(u32 size)
    {
        u8* ptr = buffer + offset;

        // Write the size of the array in the first slot
        size_t allocSize = sizeof(T) * size;
        (*ptr) = allocSize;

        offset += size;
        for (u32 i=0; i < size; ++i)
        {
            // Placement new on each slot
            new (ptr++) T();
        }
        return ptr;
    }

private:
    u8 buffer[1024 * 1024];
    u32 offset;
};
*/