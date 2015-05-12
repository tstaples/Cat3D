#include "TestApp.h"
#include "Random.h"

#include "FileBuffer.h"

#define DEBUG_INPUT 0

namespace
{
    Math::AABB testAABB(
        Math::Vector3(Random::GetF(-10.0f, 10.0f),
                      Random::GetF(-10.0f, 10.0f),
                      Random::GetF(-10.0f, 10.0f)),
        Math::Vector3(2.0f, 2.0f, 2.0f));

    static Math::Vector3 vel(1.0f, 1.0f, 0.0f);

    void DebugLogInput(const InputEvent& e)
    {
#if DEBUG_INPUT
        char buff[256];
        sprintf_s(buff, "[InputEvent] Type: %d, Value: %u, X: %d, Y: %d\n", e.type, e.value, e.x, e.y);
        OutputDebugStringA(buff);
#endif
    }
    
    void DrawGroundPlane()
    {
        const s32 groundSize = 25;
        for (s32 x = -groundSize; x <= groundSize; ++x)
        {
            Math::Vector3   a((f32)x, 0.0f, (f32)-groundSize),
                            b((f32)x, 0.0f, (f32)groundSize);
            SimpleDraw::AddLine(a, b, Color::White());
        }
        for (s32 z = -groundSize; z <= groundSize; ++z)
        {
            Math::Vector3   a((f32)-groundSize, 0.0f, (f32)z),
                            b((f32)groundSize, 0.0f, (f32)z);
            SimpleDraw::AddLine(a, b, Color::White());
        }
    }
}


//----------------------------------------------------------------------------------------------------

TestApp::TestApp()
    : mWidth(0)
    , mHeight(0)
    , mOctree(Math::AABB(Math::Vector3::Zero(), Math::Vector3(50.0f, 50.0f, 50.0f)))
    , mGameObjectPool(10)
{
    memset(mInputData.keyStates, 0, sizeof(bool) * 256);
    memset(mInputData.mouseStates, 0, sizeof(bool) * 4);
}

//----------------------------------------------------------------------------------------------------

TestApp::~TestApp()
{
}

//----------------------------------------------------------------------------------------------------

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

    // Bind controls
    mInputManager.BindAction(Mouse::LBUTTON, Input::MouseDown, MAKE_ACTION_DELEGATE(TestApp, &TestApp::OnSelectObject));
    mInputManager.BindAction(Mouse::SCROLL, Input::MouseScroll, MAKE_ACTION_DELEGATE(TestApp, &TestApp::OnZoom));

    mInputManager.BindAxis(Mouse::RBUTTON, Input::MouseDown, MAKE_AXIS_DELEGATE(TestApp, &TestApp::OnCameraLook));
    mInputManager.BindAxis(Mouse::MBUTTON, Input::MouseDown, MAKE_AXIS_DELEGATE(TestApp, &TestApp::OnPanCamera));

    // TEMP
    GameObjectHandle handle1 = mGameObjectPool.Allocate();
    GameObjectHandle handle2 = mGameObjectPool.Allocate();

    GameObject* g1 = handle1.Get();
    GameObject* g2 = handle2.Get();

    TransformComponent* t1 = new TransformComponent(g1);
    TransformComponent* t2 = new TransformComponent(g2);
    t1->Translate(Math::Vector3(15.0f, 3.0f, 5.0f));
    t2->Translate(Math::Vector3(-15.0f, 3.0f, 5.0f));

    g1->AddComponent(t1);
    g2->AddComponent(t2);

    mObjects.push_back(EditorObject(handle1));
    mObjects.push_back(EditorObject(handle2));

    f32 m[16] = 
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
    Math::Matrix mat = *(Math::Matrix*)&m;
    Math::Matrix mrot = Math::Matrix::RotationX(Math::kPiByTwo);
    Math::Vector3 rot = Math::GetRotation(mrot);

    char temp[128];
    int sz = sprintf(temp, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\0",
                    m[0], m[1], m[2], m[3],
                    m[4], m[5], m[6], m[7],
                    m[8], m[9], m[10], m[11],
                    m[12], m[13], m[14], m[15]);

    //mSelectedObjects.push_back(&mObjects[1]);

    u32 sizz = 0;
    //const char* data = GetSelectedObjectData(sz);

    //GameObject* g = new GameObject();
    //TransformComponent* t = new TransformComponent(g);
    //g->AddComponent(t);
    //const MetaClass* gc = g->GetMetaClass();
    //const MetaClass* tc = t->GetMetaClass();
    //
    //TransformComponent* tt = nullptr;
    //bool result = g->GetComponent(tt);
}

//----------------------------------------------------------------------------------------------------

void TestApp::OnTerminate()
{
    mOctree.Destroy();
    mGameObjectPool.Flush();

	SimpleDraw::Terminate();
	mGraphicsSystem.Terminate();
	mWindow.Terminate();
}

//----------------------------------------------------------------------------------------------------

bool TestApp::OnInput(const InputEvent& evt)
{
    DebugLogInput(evt);


    bool handled = false;
    switch(evt.type)
    {
    case Input::KeyDown:
        {
            if(evt.value == VK_ESCAPE)
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

void TestApp::OnUpdate()
{
	if (mWindow.HandleMessage())
	{
		mRunning = false;
		return;
	}

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
    
    //DrawGroundPlane();

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

/*
- object name (LE)
- num components
    - component name (LE)
    - num fields
    - field name (LE)
    - field size
    - field data
*/

const u8* TestApp::GetSelectedObjectData(u32& size)
{
    if (mSelectedObjects.empty())
    {
        size = 0;
        return nullptr;
    }

    memset(mObjBuffer, 0, 2048);
    SerialWriter writer(mObjBuffer, 2048);

    GameObject* gameObject = mSelectedObjects[0]->GetGameObject();
    const MetaClass* metaClass = gameObject->GetMetaClass();
    writer.WriteLengthEncodedString(metaClass->GetName());

    const std::vector<Component*>& components = gameObject->GetComponents();
    writer.Write((u32)components.size());
    for (Component* c : components)
    {
        const MetaClass* compMetaClass = c->GetMetaClass();
        writer.WriteLengthEncodedString(compMetaClass->GetName());

        const MetaField* fields = compMetaClass->GetFields();
        const u32 numFields = compMetaClass->GetNumFields();
        writer.Write(numFields);
        for (u32 i=0; i < numFields; ++i)
        {
            const MetaField* field = &fields[i];
            const u32 offset = field->GetOffset();
            const u32 fieldSize = field->GetType()->GetSize();

            writer.WriteLengthEncodedString(field->GetName());
            writer.Write(fieldSize);
            writer.WriteArray(c + offset, fieldSize);
        }
    }
    size = writer.GetSize();
    return mObjBuffer;
}

bool TestApp::OnCameraLook(s32 val)
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

bool TestApp::OnZoom()
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

bool TestApp::OnPanCamera(s32 val)
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

bool TestApp::OnSelectObject()
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