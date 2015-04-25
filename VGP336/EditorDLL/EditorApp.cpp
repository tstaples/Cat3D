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
    , mMouseX(0)
    , mMouseY(0)
    , mMouseMoveX(0)
    , mMouseMoveY(0)
    , mMouseScrollDelta(0.0f)
    , mOctree(Math::AABB(Math::Vector3::Zero(), Math::Vector3(50.0f, 50.0f, 50.0f)))
{
    memset(mKeyStates, 0, sizeof(bool) * 256);
    memset(mMouseStates, 0, sizeof(bool) * 4);
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
                // Get the offset since the last frame
                mMouseMoveX = (f32)(evt.x - mMouseX);
                mMouseMoveY = (f32)(evt.y - mMouseY);
            }
            mMouseX = evt.x;
            mMouseY = evt.y;
            handled = true;
        }
        break;
    case InputEvent::MouseDown:
        {
            mMouseStates[evt.value] = true;
            handled = true;
        }
        break;
    case InputEvent::MouseUp:
        {
            mMouseStates[evt.value] = false;
            handled = true;
        }
        break;
    case InputEvent::MouseScroll:
        {
            mMouseScrollDelta = evt.y;
            handled = true;
        }
        break;
    }
    return handled;
}

//----------------------------------------------------------------------------------------------------

void EditorApp::OnUpdate()
{
    mOctree.Destroy();
    mOctree.Insert(testAABB);
    mOctree.Debug_DrawTree();

    mTimer.Update();
	const f32 deltaTime = mTimer.GetElapsedTime();
    UpdateControls(deltaTime);


	// Render
	mGraphicsSystem.BeginRender(Color::Black());

    Color sphereCol = (temp) ? Color::Red() : Color::White();
    SimpleDraw::AddAABB(Math::Vector3::Zero(), 2.0f, sphereCol);

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

    // Object selection
    bool objectClicked = false; // used for testing below statement
    std::vector<Math::AABB> objects;

    if (mMouseStates[Mouse::LBUTTON])
    {
        Math::Ray ray = mCamera.GetMouseRay(mMouseX, mMouseY, mWidth, mHeight);
        objectClicked = mOctree.GetIntersectingObjects(ray, objects);
    }

    // Camera look
    if (mMouseStates[Mouse::RBUTTON])
    {
        mCamera.Yaw(mMouseMoveX * lookSensitivity);
        mCamera.Pitch(mMouseMoveY * lookSensitivity);
    }
    // Move camera
    if (mMouseStates[Mouse::MBUTTON])
    {
        mCamera.Strafe((-mMouseMoveX) * moveSensitivity);
        mCamera.Rise(mMouseMoveY * moveSensitivity);
    }
    // Camera zoom
    if (mMouseScrollDelta == WHEEL_SCROLL_UP)
    {
        mCamera.Walk(zoomDistance);
    }
    else if (mMouseScrollDelta == WHEEL_SCROLL_DOWN)
    {
        mCamera.Walk(-zoomDistance);
    }

    // Camera movement
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
}

//----------------------------------------------------------------------------------------------------

//Math::Vector3 EditorApp::MouseToWorld()
//{
//    Math::Vector3 mouseNDC;
//    mouseNDC.x = (2.0f * mMouseX) / mWidth - 1.0f;
//    mouseNDC.y = 1.0f - (2.0f * mMouseY) / mHeight;
//    mouseNDC.z = 0.0f;
//
//    // Transform the mouse NDC coords into world space
//    Math::Matrix projection     = mCamera.GetProjectionMatrix();
//    Math::Matrix invProjection  = Math::Inverse(projection);
//    Math::Matrix cameraView     = Math::Inverse(mCamera.GetViewMatrix());
//    Math::Matrix transform      = invProjection * cameraView;
//    Math::Vector3 mouseWorld    = Math::TransformCoord(mouseNDC, transform);
//
//    return mouseWorld;
//}
//
////----------------------------------------------------------------------------------------------------
//
//bool EditorApp::SelectedObjectInWorld(const Math::AABB& aabb)
//{
//    Math::Vector3 mouseWorld    = MouseToWorld();
//    Math::Vector3 cameraPos     = mCamera.GetPosition();
//    Math::Vector3 dir           = Math::Normalize(mouseWorld - cameraPos);
//    Math::Ray ray(cameraPos, dir);
//
//    f32 dEntry, dExit;
//    return Math::Intersect(ray, aabb, dEntry, dExit);
//}