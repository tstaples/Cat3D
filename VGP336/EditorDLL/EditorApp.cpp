#include "EditorApp.h"
#include "Random.h"

#include "Gizmo.h"
#include "InputCallbacks.h"
#include "Components.h"
#include "SimpleDraw.h"

namespace
{
    void DrawGroundPlane(s32 groundSize, s32 gap)
    {
        Color lineColour(0.75f, 0.75f, 0.75f, 1.0f);
        for (s32 x = -groundSize; x <= groundSize; x += gap)
        {
            Math::Vector3   a((f32)x, 0.0f, (f32)-groundSize),
                            b((f32)x, 0.0f, (f32)groundSize);
            SimpleDraw::AddLine(a, b, lineColour);
        }
        for (s32 z = -groundSize; z <= groundSize; z += gap)
        {
            Math::Vector3   a((f32)-groundSize, 0.0f, (f32)z),
                            b((f32)groundSize, 0.0f, (f32)z);
            SimpleDraw::AddLine(a, b, lineColour);
        }
    }
}

EditorApp::EditorApp()
    : mOctree(Math::AABB(Math::Vector3::Zero(), Math::Vector3(50.0f, 50.0f, 50.0f)))
    , mCallbacks(*this)
    , mGameWorld(this, 100)
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
    
	mTimer.Initialize();

	mGraphicsSystem.Initialize(GetWindow(), false);
	SimpleDraw::Initialize(mGraphicsSystem);

	mCamera.Setup(Math::kPiByTwo, (f32)width / (f32)height, 0.01f, 10000.0f);
	mCamera.SetPosition(Math::Vector3(0.0f, 0.0f, -50.0f));

    // Register input callbacks
    mCallbacks.RegisterCallbacks();

    mpGizmo = new TranslateGizmo(mCamera, 1.0f, 5.0f);

    // TODO: Read settings from editor
    GameSettings settings;
    settings.timeStep = 1.0f / 60.0f;
    settings.gravity = -9.8f;
    mGameWorld.OnInitialize(settings, mGraphicsSystem, mCamera);

    // Discover any objects the gameworld has on startup
    for (GameObjectHandle handle : mGameWorld.mUpdateList)
    {
        mObjects.push_back(EditorObject(handle));
    }
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
    //LOG("FPS: %f", mTimer.GetFramesPerSecond());
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
        GameObject* gameObject = object.GetGameObject();
        CameraComponent* cameraComponent = nullptr;
        if (gameObject->FindComponent(cameraComponent))
        {
            TransformComponent* transformComponent = nullptr;
            gameObject->GetComponent(transformComponent);
            Math::Vector3 pos = transformComponent->GetPosition();

            const Camera& camera = cameraComponent->GetCamera();
            Math::Matrix proj = camera.GetProjectionMatrix();
            Math::Matrix view = camera.GetViewMatrix();
            Math::Matrix transform =  view;

            //Math::Vector3 tl = Math::TransformCoord(Math::Vector3(-1.0f, -1.0f, 0.0f), transform);
            //Math::Vector3 tr = Math::TransformCoord(Math::Vector3(1.0f, -1.0f, 0.0f), transform);
            //Math::Vector3 br = Math::TransformCoord(Math::Vector3(1.0f, 1.0f, 0.0f), transform);
            //Math::Vector3 bl = Math::TransformCoord(Math::Vector3(-1.0f, 1.0f, 0.0f), transform);
            f32 hw = (f32)mWidth / 2;
            f32 hh = (f32)mHeight / 2;

            Math::Vector3 tl(-hw, -hh, 0.0f);
            Math::Vector3 tr(hw, -hh, 0.0f);
            Math::Vector3 br(hw, hh, 0.0f);
            Math::Vector3 bl(-hw, hh, 0.0f);

            tl = Math::TransformCoord(tl, transform);
            tr = Math::TransformCoord(tr, transform);
            br = Math::TransformCoord(br, transform);
            bl = Math::TransformCoord(bl, transform);

            SimpleDraw::AddLine(tl, tr, Color::Green());
            SimpleDraw::AddLine(tr, br, Color::Green());
            SimpleDraw::AddLine(bl, br, Color::Green());
            SimpleDraw::AddLine(bl, tl, Color::Green());
            
            SimpleDraw::AddLine(pos, tl, Color::Green());
            SimpleDraw::AddLine(pos, tr, Color::Green());
            SimpleDraw::AddLine(pos, bl, Color::Green());
            SimpleDraw::AddLine(pos, br, Color::Green());

        }

        if (object.IsSelected())
        {
            // Draw the gizmo if any object is selected
            drawGizmo = true;

            // Draw the collider
            object.Draw();
        }
    }

	// Render
	mGraphicsSystem.BeginRender(Color::Black());

    DrawGroundPlane(100, 5);

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
	mCamera.Setup(Math::kPiByTwo, (f32)mWidth / (f32)mHeight, 0.01f, 10000.0f);
}