#include "TestApp.h"
#include "Random.h"


#define DEBUG_INPUT 1

namespace
{
    static Math::Vector3 vel(1.0f, 1.0f, 0.0f);

    void DebugLogInput(const InputEvent& e)
    {
#if DEBUG_INPUT
        char buff[256];
        sprintf_s(buff, "[InputEvent] Type: %d, Value: %u, X: %d, Y: %d\n", e.type, e.value, e.x, e.y);
        OutputDebugStringA(buff);
#endif
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

    void TestPath()
    {
        Path path("../Data/Stuff/soldier.catm");

        std::wstring extension = path.GetExtension();
        ASSERT(extension.compare(L"catm") == 0, "GetExtension() Failed");

        std::wstring filename = path.GetFileName();
        ASSERT(filename.compare(L"soldier.catm") == 0, "GetFileName() Failed");

        std::wstring filenameNoExt = path.GetFileNameWithoutExtension();
        ASSERT(filenameNoExt.compare(L"soldier") == 0, "GetFileNameWithoutExtension() Failed");

        std::wstring originalPath = path.GetPath();
        ASSERT(originalPath.compare(L"../Data/Stuff/soldier.catm") == 0, "GetPath() Failed");

        std::string originalPathString = path.GetPathString();
        ASSERT(originalPathString.compare("../Data/Stuff/soldier.catm") == 0, "GetPathString() Failed");
    }
}

TestApp::TestApp()
    : mWidth(0)
    , mHeight(0)
    , mMouseX(0)
    , mMouseY(0)
    , mMouseMoveX(0)
    , mMouseMoveY(0)
    , mMouseScrollDelta(0)
    , mOctree(Math::AABB(Math::Vector3::Zero(), Math::Vector3(50.0f, 50.0f, 50.0f)))
{
    memset(mKeyStates, 0, sizeof(bool) * 256);
    memset(mMouseStates, 0, sizeof(bool) * 4);
}

TestApp::~TestApp()
{
}

void TestApp::OnInitialize(u32 width, u32 height)
{
    // Store width and height for later use
    // TODO: update when resize event occurs
    mWidth = width;
    mHeight = height;

	// Init the window
	mWindow.Initialize(GetInstance(), GetAppName(), width, height);
	HookWindow(mWindow.GetWindowHandle());

	mTimer.Initialize();

	mGraphicsSystem.Initialize(mWindow.GetWindowHandle(), false);
	SimpleDraw::Initialize(mGraphicsSystem);

	mCamera.Setup(Math::kPiByTwo, (f32)width / (f32)height, 0.01f, 10000.0f);
	mCamera.SetPosition(Math::Vector3(0.0f, 0.0f, -100.0f));

    testAABB = Math::AABB(
        Math::Vector3(Random::GetF(-10.0f, 10.0f),
                      Random::GetF(-10.0f, 10.0f),
                      Random::GetF(-10.0f, 10.0f)),
        Math::Vector3(2.0f, 2.0f, 2.0f));

    mAxisMap.insert(std::make_pair(&mMouseStates[Mouse::RBUTTON], Delegate0<void>::Make<TestApp, &TestApp::OnMouseRightClick>(this)));
}

void TestApp::OnTerminate()
{
    mOctree.Destroy();

	SimpleDraw::Terminate();
	mGraphicsSystem.Terminate();
	mWindow.Terminate();
}

bool TestApp::OnInput(const InputEvent& evt)
{
    DebugLogInput(evt);


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
                if (Math::Abs(mMouseMoveX) < 1.0f) mMouseMoveX = 0.0f;
                if (Math::Abs(mMouseMoveY) < 1.0f) mMouseMoveY = 0.0f;
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
            mMouseScrollDelta = evt.wheeldelta;
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
		return;
	}

	mTimer.Update();
	const f32 deltaTime = mTimer.GetElapsedTime();

    // Apply some velocity to move the cube
    mOctree.Destroy();
    mOctree.Insert(testAABB);
    mOctree.Debug_DrawTree();

	// Camera movement modifiers (TODO: Make these configurable)
	const f32 kMoveSpeed = 1.0f;
	const f32 lookSensitivity = 0.0025f;
    const f32 moveSensitivity = 0.0025f;
    const f32 zoomDistance = 1.0f;

    for (auto it : mAxisMap)
    {
        if (*it.first)
        {
            it.second();
        }
    }

    // Camera look
    //if (mMouseStates[Mouse::RBUTTON])
    //{
    //    mCamera.Yaw(mMouseMoveX * lookSensitivity);
    //    mCamera.Pitch(mMouseMoveY * lookSensitivity);
    //}
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
    // Reset to prevent infinite zoom
    mMouseScrollDelta = 0;

    bool objectClicked = false; // used for testing below statement
    std::vector<Math::AABB> objects;

    if (mMouseStates[Mouse::LBUTTON])
    {
        Math::Ray ray = mCamera.GetMouseRay(mMouseX, mMouseY, mWidth, mHeight);
        objectClicked = mOctree.GetIntersectingObjects(ray, objects);
    }

    // Player movement
    //if(mKeyStates['W'])
    //{
    //    mCamera.Walk(kMoveSpeed * deltaTime);
    //}
    //else if(mKeyStates['S'])
    //{
    //    mCamera.Walk(-kMoveSpeed * deltaTime);
    //}
    //else if(mKeyStates['D'])
    //{
    //    mCamera.Strafe(kMoveSpeed * deltaTime);
    //}
    //else if(mKeyStates['A'])
    //{
    //    mCamera.Strafe(-kMoveSpeed * deltaTime);
    //}

	// Render
	mGraphicsSystem.BeginRender(Color::Black());
    
    if (objectClicked)
    {
        for (auto it : objects)
        {
            SimpleDraw::AddAABB(it.center, it.extend.x, Color::Red());
        }
    }
    else
    {
        SimpleDraw::AddAABB(testAABB.center, testAABB.extend.x, Color::White());
    }

	SimpleDraw::Render(mCamera);
	mGraphicsSystem.EndRender();
}

void TestApp::OnMouseRightClick()
{
    // TODO: Smoothing by interpolating between desired angle and current
    // (reference steering behaviours)
	const f32 lookSensitivity = 0.025f;

    mCamera.Yaw(mMouseMoveX * lookSensitivity);
    mCamera.Pitch(mMouseMoveY * lookSensitivity);

    // Zero out mouse move so the camera doesn't continue to rotate
    mMouseMoveX = 0.0f;
    mMouseMoveY = 0.0f;
}

//Math::Vector3 TestApp::MouseToWorld()
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

bool TestApp::SelectedObjectInWorld(const Math::AABB& aabb)
{
    Math::Ray ray(mCamera.GetMouseRay(mMouseX, mMouseY, mWidth, mHeight));
    return Math::Intersect(ray, aabb);
}

//Math::Ray TestApp::GetMouseRay()
//{
//    Math::Vector3 mouseWorld    = MouseToWorld();
//    Math::Vector3 cameraPos     = mCamera.GetPosition();
//    Math::Vector3 dir           = Math::Normalize(mouseWorld - cameraPos);
//    return Math::Ray(cameraPos, dir);
//}


/*
    // http://www.gamedev.net/blog/355/entry-2250186-designing-a-robust-input-handling-system-for-games/

    Input types:
    1. Actions - 1 input : 1 output
    2. States - continous action (ie. running)
    3. Ranges - joystick

    Input context:
    - each context defines an input map (std::map<keycode, name>)
        - raw keycode is mapped to the higher level action (ie. w mapped to "run")
        

    - Input callbacks will return a flag if the input was handled
        - if it returns false, the input will be passed down to the next handler

    class InputManager
    {
        // Ordered set or something might be preferred to prioritize certain contexts
        std::vector<InputContext> mContexts;
    };

    Handlers:
    - Editor
        - editor controls (camera movement, object selection etc.)
    - Game
        - Additional contexts can be created by the user (ie. main menu input, game etc.)
        - This layer would be responsible for passing the input down to any of these additional layers


*/