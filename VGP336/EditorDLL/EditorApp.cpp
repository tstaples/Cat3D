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
    , mGameObjectPool(10)
{
    memset(mInputData.keyStates, 0, sizeof(bool) * 256);
    memset(mInputData.mouseStates, 0, sizeof(bool) * 4);
    memset(mObjBuffer, 0, 2048);
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
    GameObjectHandle handle1 = mGameObjectPool.Allocate();
    GameObjectHandle handle2 = mGameObjectPool.Allocate();

    GameObject* g1 = handle1.Get();
    GameObject* g2 = handle2.Get();

    // leak
    TransformComponent* t1 = new TransformComponent(g1);
    TransformComponent* t2 = new TransformComponent(g2);
    t1->Translate(Math::Vector3(15.0f, 3.0f, 5.0f));
    t2->Translate(Math::Vector3(-15.0f, 3.0f, 5.0f));

    g1->AddComponent(t1);
    g2->AddComponent(t2);

    mObjects.push_back(EditorObject(handle1));
    mObjects.push_back(EditorObject(handle2));
}

//----------------------------------------------------------------------------------------------------

void EditorApp::OnTerminate()
{
    mOctree.Destroy();
    mObjects.clear();
    mGameObjectPool.Flush();

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
    for (u32 i=0; i < mObjects.size(); ++i)
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
                //SimpleDraw::AddLine(object.GetPosition(), translation * 15.0f, Color::Cyan());
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

const u8* EditorApp::GetSelectedObjectData(u32& size)
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
            writer.Write(field->GetType());
            writer.Write(fieldSize);
            writer.WriteArray(fieldData, fieldSize);
        }
    }
    size = writer.GetOffset();
    return mObjBuffer;
}

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