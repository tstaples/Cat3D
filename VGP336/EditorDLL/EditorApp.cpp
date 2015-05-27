#include "EditorApp.h"
#include "Random.h"

#include "Gizmo.h"
#include "InputCallbacks.h"

EditorApp::EditorApp()
    : mWidth(0)
    , mHeight(0)
    , mOctree(Math::AABB(Math::Vector3::Zero(), Math::Vector3(50.0f, 50.0f, 50.0f)))
    , mCallbacks(*this)
    , mGameWorld(100)
    , mIsGameRunning(false)
{
    memset(mInputData.keyStates, 0, sizeof(bool) * 256);
    memset(mInputData.mouseStates, 0, sizeof(bool) * 4);
}

//----------------------------------------------------------------------------------------------------

EditorApp::~EditorApp()
{
}

//----------------------------------------------------------------------------------------------------

void EditorApp::OnInitialize(u32 width, u32 height)
{
    Meta::MetaRegistration();
    
    // Store width and height for later use
    // TODO: update when resize event occurs
    mWidth = width;
    mHeight = height;

	mTimer.Initialize();

	mGraphicsSystem.Initialize(GetWindow(), false);
	SimpleDraw::Initialize(mGraphicsSystem);

	mCamera.Setup(Math::kPiByTwo, (f32)width / (f32)height, 0.01f, 10000.0f);
	mCamera.SetPosition(Math::Vector3(0.0f, 0.0f, -50.0f));

    // Register input callbacks
    mCallbacks.RegisterCallbacks();

    mpGizmo = new TranslateGizmo(mCamera, 1.0f, 5.0f);

    GameSettings settings;
    mGameWorld.OnInitialize(settings, mGraphicsSystem, mCamera);

    //GameObjectHandle handle = mFactory.Create("../Data/GameObjects/testcube.json");
    //GameObject* go = handle.Get();
    //mObjects.push_back(EditorObject(handle));
}

//----------------------------------------------------------------------------------------------------

void EditorApp::OnTerminate()
{
    VERIFY(mGameWorld.OnShutdown(), "[EditorApp] GameWorld failed to shutdown");

    SafeDelete(mpGizmo);
    mOctree.Destroy();
    mObjects.clear();
	SimpleDraw::Terminate();
	mGraphicsSystem.Terminate();
}

//----------------------------------------------------------------------------------------------------

bool EditorApp::OnInput(const InputEvent& evt)
{
    bool handled = false;
    switch(evt.type)
    {
    case Input::KeyDown:
        {
            if(evt.value == VK_ESCAPE && IsOwner())
            {
                PostQuitMessage(0);
            }
            mInputData.keyStates[evt.value] = true;
            handled = true;
        }
        break;
    case Input::KeyUp:
        {
            mInputData.keyStates[evt.value] = false;
            handled = true;
        }
        break;
    case Input::MouseMove:
        {
            if (mInputData.mouseX != -1 && mInputData.mouseY != -1)
            {
                // Get the offset since the last frame
                mInputData.mouseMoveX = (f32)(evt.x - mInputData.mouseX);
                mInputData.mouseMoveY = (f32)(evt.y - mInputData.mouseY);
            }
            mInputData.mouseX = evt.x;
            mInputData.mouseY = evt.y;
            handled = true;
        }
        break;
    case Input::MouseDown:
        {
            mInputData.mouseStates[evt.value] = true;
            handled = true;
        }
        break;
    case Input::MouseUp:
        {
            mInputData.mouseStates[evt.value] = false;
            handled = true;
        }
        break;
    case Input::MouseScroll:
        {
            mInputData.mouseScrollDelta = evt.wheeldelta;
            handled = true;
        }
        break;
    }

    mInputManager.OnAction(evt);

    return handled;
}

//----------------------------------------------------------------------------------------------------

void EditorApp::OnUpdate()
{
    mInputManager.Update(mInputData);
	
    // Set time to 0 when not running
    mTimer.Update();
	const f32 deltaTime = (mIsGameRunning) ? mTimer.GetElapsedTime() : 0.0f;
    mGameWorld.OnUpdate(deltaTime);

    // Destroy and re-create the entire tree
    mOctree.Destroy();
    for (u32 i=0; i < mObjects.size(); ++i)
    {
        mOctree.Insert(mObjects[i], mObjects[i].GetCollider());
    }
    //mOctree.Debug_DrawTree();

    // Update the editor objects
    bool drawGizmo = false;
    for (auto object : mObjects)
    {
        Color col = Color::White();
        if (object.IsSelected())
        {
            drawGizmo = true;
            col = Color::Red();
        }
        //SimpleDraw::AddAABB(object.GetCollider(), col);
    }

	// Render
	mGraphicsSystem.BeginRender(Color::Black());

    mGameWorld.OnRender();

    if (drawGizmo)
    {
        mpGizmo->Draw(mSelectedObjects);
    }

	SimpleDraw::Render(mCamera);
	mGraphicsSystem.EndRender();
}

//----------------------------------------------------------------------------------------------------

void EditorApp::OnResizeWindow()
{
    mGraphicsSystem.Resize(mWidth, mHeight);
}