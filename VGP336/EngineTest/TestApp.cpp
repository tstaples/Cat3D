#include "TestApp.h"
#include "Random.h"

#include "FileBuffer.h"
#include "Gizmo.h"

#define DEBUG_INPUT 0

namespace
{
    const char* types[7] =
    {
        "Character",
		"KeyUp",
		"KeyDown",
		"MouseUp",
		"MouseDown",
		"MouseMove",
        "MouseScroll"
    };

    void DebugLogInput(const InputEvent& e, const InputData& d)
    {
#if DEBUG_INPUT
        char buff[256];
        if (e.type == 5 && d.mouseMoveX != 0.0f || d.mouseMoveY != 0.0f)
        {
            sprintf_s(buff, "[InputEvent] Type: %s, Value: %u, dist moved in X: %f, Y: %f\n", types[e.type], e.value, d.mouseMoveX, d.mouseMoveY);
        }
        else
        {
            sprintf_s(buff, "[InputEvent] Type: %s, Value: %u, X: %d, Y: %d\n", types[e.type], e.value, e.x, e.y);
        }
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
    , mpGizmo(nullptr)
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

    mpGizmo = new TranslateGizmo(mCamera, 15.0f, 5.0f);

    // Bind controls
    mInputManager.BindAction(Mouse::LBUTTON, Input::MouseDown, MAKE_ACTION_DELEGATE(TestApp, &TestApp::OnSelectObject));
    mInputManager.BindAction(Mouse::SCROLL, Input::MouseScroll, MAKE_ACTION_DELEGATE(TestApp, &TestApp::OnZoom));

    mInputManager.BindAxis(Mouse::LBUTTON, Input::MouseDown, MAKE_AXIS_DELEGATE(TestApp, &TestApp::OnMouseDrag));
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

    //mSelectedObjects.push_back(&mObjects[1]);

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
    SafeDelete(mpGizmo);
    mOctree.Destroy();
    mGameObjectPool.Flush();

	SimpleDraw::Terminate();
	mGraphicsSystem.Terminate();
	mWindow.Terminate();
}

//----------------------------------------------------------------------------------------------------

bool TestApp::OnInput(const InputEvent& evt)
{

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

    DebugLogInput(evt, mInputData);
    mInputManager.OnAction(evt);

    return handled;
}

//----------------------------------------------------------------------------------------------------
bool doneonce = false;
bool doneonce2 = false;

void TestApp::OnUpdate()
{
	if (mWindow.HandleMessage())
	{
		mRunning = false;
		return;
	}
    mInputManager.Update(mInputData);

	mTimer.Update();
	const f32 deltaTime = mTimer.GetElapsedTime();

    //if (mInputData.keyStates['W'] && !doneonce)
    //{
    //    u32 size = 0;
    //    const u8* temp = GetSelectedObjectData(size);
    //    TransformComponent* tcomp = nullptr;
    //    if (mObjects[1].GetGameObject()->GetComponent(tcomp))
    //    {
    //        tcomp->Translate(Math::Vector3(20.0f, -10.0f, 1.0f));

    //        u8 tempBuff[2048];
    //        SerialWriter w(tempBuff, 2048);
    //        w.Write(mObjects[1].GetHandle().GetIndex());
    //        w.Write(mObjects[1].GetHandle().GetInstance());
    //        w.WriteLengthEncodedString(tcomp->GetMetaClass()->GetName());
    //        for (u32 i=0; i < 3; ++i)
    //        {
    //            MetaField field = tcomp->GetMetaClass()->GetFields()[i];
    //            u32 foffset = field.GetOffset();
    //            u32 fsize = field.GetType()->GetSize();
    //            char* fieldData = ((char*)tcomp) + foffset;
    //            w.WriteArray(fieldData, fsize);
    //        }
    //        UpdateComponent(tempBuff, w.GetOffset());
    //    }
    //    doneonce = true;
    //}
    //if (mInputData.keyStates['E'] && !doneonce2)
    //{
    //    u32 size = 0;
    //    const u8* temp = GetSelectedObjectData(size);
    //    TransformComponent* tcomp = nullptr;
    //    if (mObjects[0].GetGameObject()->GetComponent(tcomp))
    //    {
    //        tcomp->Translate(Math::Vector3(0.0f, -20.0f, 0.0f));

    //        u8 tempBuff[2048];
    //        SerialWriter w(tempBuff, 2048);
    //        w.Write(mObjects[0].GetHandle().GetIndex());
    //        w.Write(mObjects[0].GetHandle().GetInstance());
    //        w.WriteLengthEncodedString(tcomp->GetMetaClass()->GetName());
    //        for (u32 i=0; i < 3; ++i)
    //        {
    //            MetaField field = tcomp->GetMetaClass()->GetFields()[i];
    //            u32 foffset = field.GetOffset();
    //            u32 fsize = field.GetType()->GetSize();
    //            char* fieldData = ((char*)tcomp) + foffset;
    //            w.WriteArray(fieldData, fsize);
    //        }
    //        UpdateComponent(tempBuff, w.GetOffset());
    //    }
    //    doneonce2 = true;
    //}


    // Destroy and re-create the entire tree
    mOctree.Destroy();
    for (int i=0; i < mObjects.size(); ++i)
    {
        mOctree.Insert(mObjects[i], mObjects[i].GetCollider());
    }
    mOctree.Debug_DrawTree();


	// Render
	mGraphicsSystem.BeginRender(Color::Black());
    
    //DrawGroundPlane();

    for (auto object : mObjects)
    {
        Color col = Color::White();
        if (object.IsSelected())
        {
            col = Color::Red();
        }
        SimpleDraw::AddAABB(object.GetCollider(), col);
    }
    // Draw the gizmo on top of everything
    mpGizmo->Draw(mSelectedObjects);

	SimpleDraw::Render(mCamera);
	mGraphicsSystem.EndRender();

    mInputData.mouseMoveX = 0.0f;
    mInputData.mouseMoveY = 0.0f;
}

//----------------------------------------------------------------------------------------------------

const u8* TestApp::GetSelectedObjectData(u32& size)
{
    if (mSelectedObjects.empty())
    {
        size = 0;
        return nullptr;
    }

    memset(mObjBuffer, 0, 2048);
    SerialWriter writer(mObjBuffer, 2048);

    EditorObject* editorObject = mSelectedObjects[0];
    GameObject* gameObject = editorObject->GetGameObject();
    const MetaClass* metaClass = gameObject->GetMetaClass();

    writer.WriteLengthEncodedString(metaClass->GetName());
    writer.Write(editorObject->GetHandle().GetIndex()); // used as object identifier
    writer.Write(editorObject->GetHandle().GetInstance());

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
            char* fieldData = ((char*)c) + offset;

            writer.WriteLengthEncodedString(field->GetName());
            writer.Write(field->GetType()->GetType());
            writer.Write(fieldSize);
            writer.Write(offset);
            writer.WriteArray(fieldData, fieldSize);
        }
    }
    size = writer.GetOffset();
    return mObjBuffer;
}
void TestApp::UpdateComponent(const u8* buffer, u32 buffsize)
{
    SerialReader reader((u8*)buffer, buffsize);

    // Get handle data to find corresponding gameobject
    u16 index = reader.Read<u16>();
    u16 instance = reader.Read<u16>();
    GameObjectHandle handle(instance, index);
    GameObject* gameObject = mGameObjectPool.Get(handle);

    std::string compName = reader.ReadLengthEncodedString();
    const std::vector<Component*>& components = gameObject->GetComponents();
    for (Component* c : components)
    {
        // Find the component by name
        const MetaClass* compMetaClass = c->GetMetaClass();
        if (compName.compare(compMetaClass->GetName()) == 0)
        {
            // Read remaining data (component data)
            u32 sz = buffsize - reader.GetOffset();
            char data[2048];

            const MetaField* fields = compMetaClass->GetFields();
            const u32 numFields = compMetaClass->GetNumFields();
            for (u32 i=0; i < numFields; ++i)
            {
                u32 offset = fields[i].GetOffset();
                u32 fieldSize = fields[i].GetType()->GetSize();
                reader.ReadArray(data, fieldSize);

                char* cdata = ((char*)c) + offset;
                memcpy(cdata, data, fieldSize);
            }
            break;
        }
    }
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
    
    Math::Ray ray = mCamera.GetMouseRay(mInputData.mouseX, mInputData.mouseY, mWidth, mHeight);
    if (!mpGizmo->IsSelected(mSelectedObjects, ray))
    {
        // De-select the objects and remove any existing objects
        // TODO: check if shift or w/e is pressed for multiple selection
        for (auto object : mSelectedObjects)
        {
            object->DeSelect();
        }
        mSelectedObjects.clear();

        // Convert the mouse click into a ray cast in world space and test collision
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
    }
    return handled;
}

bool TestApp::OnMouseDrag(s32 val)
{
    if (val == 0)
    {
        return false;
    }
    mpGizmo->Update(mSelectedObjects, mInputData, mWidth, mHeight);
    return true;
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