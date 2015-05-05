#include "EditorApp.h"
#include "Random.h"

namespace
{
    Math::AABB testAABB(
        Math::Vector3(Random::GetF(-10.0f, 10.0f),
                      Random::GetF(-10.0f, 10.0f),
                      Random::GetF(-10.0f, 10.0f)),
        Math::Vector3(2.0f, 2.0f, 2.0f));
}

EditorApp::EditorApp()
    : mWidth(0)
    , mHeight(0)
    , mOctree(Math::AABB(Math::Vector3::Zero(), Math::Vector3(50.0f, 50.0f, 50.0f)))
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
    // Store width and height for later use
    // TODO: update when resize event occurs
    mWidth = width;
    mHeight = height;

	mTimer.Initialize();

	mGraphicsSystem.Initialize(GetWindow(), false);
	SimpleDraw::Initialize(mGraphicsSystem);

	mCamera.Setup(Math::kPiByTwo, (f32)width / (f32)height, 0.01f, 10000.0f);
	mCamera.SetPosition(Math::Vector3(0.0f, 0.0f, -10.0f));

    // Bind controls
    mInputManager.BindAction(Mouse::LBUTTON, Input::MouseDown, MAKE_ACTION_DELEGATE(EditorApp, &EditorApp::OnSelectObject));
    mInputManager.BindAction(Mouse::SCROLL, Input::MouseScroll, MAKE_ACTION_DELEGATE(EditorApp, &EditorApp::OnZoom));

    mInputManager.BindAxis(Mouse::RBUTTON, Input::MouseDown, MAKE_AXIS_DELEGATE(EditorApp, &EditorApp::OnCameraLook));
    mInputManager.BindAxis(Mouse::MBUTTON, Input::MouseDown, MAKE_AXIS_DELEGATE(EditorApp, &EditorApp::OnPanCamera));

    // TEMP
    mObjects.push_back(EditorObject(new GameObject()));
    mObjects.push_back(EditorObject(new GameObject()));
    mObjects[0].Translate(Math::Vector3(15.0f, 3.0f, 5.0f));
    mObjects[1].Translate(Math::Vector3(-15.0f, 3.0f, 5.0f));
}

//----------------------------------------------------------------------------------------------------

void EditorApp::OnTerminate()
{
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

    // Destroy and re-create the entire tree
    mOctree.Destroy();
    for (int i=0; i < mObjects.size(); ++i)
    {
        mOctree.Insert(mObjects[i], mObjects[i].GetCollider());
    }
    mOctree.Debug_DrawTree();

    mInputManager.Update(mInputData);


    // Player movement
    const f32 kMoveSpeed = 20.0f;
    if(mInputData.keyStates['W'])
    {
        mCamera.Walk(kMoveSpeed * deltaTime);
    }
    else if(mInputData.keyStates['S'])
    {
        mCamera.Walk(-kMoveSpeed * deltaTime);
    }
    else if(mInputData.keyStates['D'])
    {
        mCamera.Strafe(kMoveSpeed * deltaTime);
    }
    else if(mInputData.keyStates['A'])
    {
        mCamera.Strafe(-kMoveSpeed * deltaTime);
    }

	// Render
	mGraphicsSystem.BeginRender(Color::Black());
    
    for (auto object : mObjects)
    {
        Color col = Color::White();
        if (object.IsSelected())
        {
            if (mInputData.mouseStates[Mouse::LBUTTON])
            {
                Math::Ray mouseRay = mCamera.GetMouseRay(mInputData.mouseX, mInputData.mouseY, mWidth, mHeight);
                Math::Vector3 translation = object.GetSelectedAxis(mouseRay);
                SimpleDraw::AddLine(object.GetPosition(), translation * 15.0f, Color::Cyan());
                //object.Translate(translation * mInputData.mouseMoveX);
            }

            object.DrawGizmo();
            col = Color::Red();
        }
        SimpleDraw::AddAABB(object.GetCollider(), col);
    }

	SimpleDraw::Render(mCamera);
	mGraphicsSystem.EndRender();
}

//----------------------------------------------------------------------------------------------------

void EditorApp::UpdateControls(f32 deltaTime)
{
	// Camera movement modifiers (TODO: Make these configurable)
    const f32 kMoveSpeed = 10.0f;
	const f32 lookSensitivity = 0.5f;
    const f32 moveSensitivity = 0.25f;
    const f32 zoomDistance = 1.0f;
}

//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------

bool EditorApp::OnCameraLook(s32 val)
{
    if (val == 0)
    {
        return false;
    }

    // TODO: Smoothing by interpolating between desired angle and current
    // (reference steering behaviours)
	const f32 lookSensitivity = 0.25f;

    mCamera.Yaw(mInputData.mouseMoveX * lookSensitivity);
    mCamera.Pitch(mInputData.mouseMoveY * lookSensitivity);

    // Zero out mouse move so the camera doesn't continue to rotate
    mInputData.mouseMoveX = 0.0f;
    mInputData.mouseMoveY = 0.0f;

    return true;
}

//----------------------------------------------------------------------------------------------------

bool EditorApp::OnZoom()
{
    const f32 zoomDistance = 2.0f;
    s8 delta = mInputData.mouseScrollDelta;
    if (delta == WHEEL_SCROLL_UP)
    {
        mCamera.Walk(zoomDistance);
    }
    else if (delta == WHEEL_SCROLL_DOWN)
    {
        mCamera.Walk(-zoomDistance);
    }
    // Reset to prevent infinite zoom
    delta = 0;

    return true;
}

//----------------------------------------------------------------------------------------------------

bool EditorApp::OnPanCamera(s32 val)
{
    if (val == 0)
    {
        return false;
    }

    const f32 moveSensitivity = 0.25f;
    mCamera.Strafe((-mInputData.mouseMoveX) * moveSensitivity);
    mCamera.Rise(mInputData.mouseMoveY * moveSensitivity);

    mInputData.mouseMoveX = 0.0f;
    mInputData.mouseMoveY = 0.0f;
    return true;
}

//----------------------------------------------------------------------------------------------------

bool EditorApp::OnSelectObject()
{
    bool handled = true;
    
    // De-select the objects
    // Remove any existing objects
    // TODO: check if shift or w/e is pressed for multiple selection
    for (auto object : mSelectedObjects)
    {
        object->DeSelect();
    }
    mSelectedObjects.clear();

    // Convert the mouse click into a ray cast in world space and test collision
    Math::Ray ray = mCamera.GetMouseRay(mInputData.mouseX, mInputData.mouseY, mWidth, mHeight);
    if (!mOctree.GetIntersectingObjects(ray, mSelectedObjects))
    {
        return handled;
    }

    // Invert the selection flag so previously selected objects are now de-selected
    std::vector<EditorObject*>::iterator it = mSelectedObjects.begin();
    for (it; it != mSelectedObjects.end(); ++it)
    {
        EditorObject* object = *it;

        // Select the object if it hasn't been already
        if (!object->IsSelected())
        {
            object->Select();
        }
    }
    return handled;
}