#include "TestApp.h"
#include "Random.h"

#define DEBUG_INPUT 0

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
}

TestApp::TestApp()
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
	mCamera.SetPosition(Math::Vector3(0.0f, 0.0f, -10.0f));

    testAABB = Math::AABB(
        Math::Vector3(Random::GetF(-10.0f, 10.0f),
                      Random::GetF(-10.0f, 10.0f),
                      Random::GetF(-10.0f, 10.0f)),
        Math::Vector3(2.0f, 2.0f, 2.0f));
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
            // TODO
            mMouseScrollDelta = (f32)evt.y;
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
    //testAABB.center += vel * deltaTime;
    mOctree.Destroy();
    mOctree.Insert(testAABB);

	// Camera movement modifiers (TODO: Make these configurable)
	const f32 kMoveSpeed = 1.0f;
	const f32 lookSensitivity = 0.0025f;
    const f32 moveSensitivity = 0.0025f;

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
    // Camera zoom (TODO)
    if (mMouseScrollDelta > 0)
    {
    }

    bool objectClicked = false; // used for testing below statement
    std::vector<Math::AABB> objects;

    if (mMouseStates[Mouse::LBUTTON])
    {
        Math::Ray ray = GetMouseRay();
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

	//SimpleDraw::AddSphere(Math::Vector3(-2.0f, 0.0f, 0.0f), 2.0f, Color::White());
	//SimpleDraw::AddSphere(Math::Vector3(2.0f, 0.0f, 0.0f), 2.0f, Color::White());

	SimpleDraw::Render(mCamera);
	mGraphicsSystem.EndRender();
}

Math::Vector3 TestApp::MouseToWorld()
{
    Math::Vector3 mouseNDC;
    mouseNDC.x = (2.0f * mMouseX) / mWidth - 1.0f;
    mouseNDC.y = 1.0f - (2.0f * mMouseY) / mHeight;
    mouseNDC.z = 0.0f;

    // Transform the mouse NDC coords into world space
    Math::Matrix projection     = mCamera.GetProjectionMatrix();
    Math::Matrix invProjection  = Math::Inverse(projection);
    Math::Matrix cameraView     = Math::Inverse(mCamera.GetViewMatrix());
    Math::Matrix transform      = invProjection * cameraView;
    Math::Vector3 mouseWorld    = Math::TransformCoord(mouseNDC, transform);

    return mouseWorld;
}

bool TestApp::SelectedObjectInWorld(const Math::AABB& aabb)
{
    Math::Ray ray(GetMouseRay());
    f32 dEntry, dExit;
    return Math::Intersect(ray, aabb, dEntry, dExit);
}

Math::Ray TestApp::GetMouseRay()
{
    Math::Vector3 mouseWorld    = MouseToWorld();
    Math::Vector3 cameraPos     = mCamera.GetPosition();
    Math::Vector3 dir           = Math::Normalize(mouseWorld - cameraPos);
    return Math::Ray(cameraPos, dir);
}