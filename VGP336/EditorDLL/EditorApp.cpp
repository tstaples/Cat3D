#include "EditorApp.h"
#include "Random.h"

#include "Gizmo.h"
#include "InputCallbacks.h"

EditorApp::EditorApp()
    : mWidth(0)
    , mHeight(0)
    , mOctree(Math::AABB(Math::Vector3::Zero(), Math::Vector3(50.0f, 50.0f, 50.0f)))
    , mGameObjectPool(100)
    , mCallbacks(*this)
    , mFactory(mGameObjectPool)
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

    mRenderService.Initialize(mGraphicsSystem, mCamera);
    // TODO: find a nicer way to do this
    Services services =
    {
        &mRenderService
    };
    mFactory.Initialize(services);

    // Register input callbacks
    mCallbacks.RegisterCallbacks();

    mpGizmo = new TranslateGizmo(mCamera, 1.0f, 5.0f);

    GameObjectHandle handle = mFactory.Create("../Data/GameObjects/testcube.json");
    GameObject* go = handle.Get();
    mObjects.push_back(EditorObject(handle));

    // TEMP
    //GameObjectHandle handle1 = mGameObjectPool.Allocate();
    //GameObjectHandle handle2 = mGameObjectPool.Allocate();
    //GameObject* g1 = handle1.Get();
    //GameObject* g2 = handle2.Get();
    //g1->SetName("Object 1");
    //g2->SetName("Object 2");
    //// leak
    //TransformComponent* t1 = new TransformComponent();
    //TransformComponent* t2 = new TransformComponent();
    //t1->Translate(Math::Vector3(15.0f, 3.0f, 5.0f));
    //t2->Translate(Math::Vector3(-15.0f, 3.0f, 5.0f));
    //ColliderComponent* col = new ColliderComponent();
    //col->SetBoundary(Math::AABB(t1->GetPosition(), Math::Vector3(5.0f, 5.0f, 5.0f)));
    //g1->AddComponent(t1);
    //g1->AddComponent(col);
    //g2->AddComponent(t2);
    //mObjects.push_back(EditorObject(handle1));
    //mObjects.push_back(EditorObject(handle2));
}

//----------------------------------------------------------------------------------------------------

void EditorApp::OnTerminate()
{
    SafeDelete(mpGizmo);
    mOctree.Destroy();
    mObjects.clear();
    mGameObjectPool.Flush();

    mRenderService.Terminate();
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
	mTimer.Update();
	const f32 deltaTime = mTimer.GetElapsedTime();

    mInputManager.Update(mInputData);

    // Destroy and re-create the entire tree
    mOctree.Destroy();
    for (u32 i=0; i < mObjects.size(); ++i)
    {
        mOctree.Insert(mObjects[i], mObjects[i].GetCollider());
    }
    //mOctree.Debug_DrawTree();


    // Player movement
    //const f32 kMoveSpeed = 20.0f;
    //if(mInputData.keyStates['W'])
    //{
    //    mCamera.Walk(kMoveSpeed * deltaTime);
    //}
    //else if(mInputData.keyStates['S'])
    //{
    //    mCamera.Walk(-kMoveSpeed * deltaTime);
    //}
    //else if(mInputData.keyStates['D'])
    //{
    //    mCamera.Strafe(kMoveSpeed * deltaTime);
    //}
    //else if(mInputData.keyStates['A'])
    //{
    //    mCamera.Strafe(-kMoveSpeed * deltaTime);
    //}

    bool drawGizmo = false;
    for (auto object : mObjects)
    {
        // Update the components
        GameObject* gameObject = object.GetGameObject();
        gameObject->Update(deltaTime);

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

    mRenderService.Update();

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