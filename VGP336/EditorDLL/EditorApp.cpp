#include "EditorApp.h"
#include "Random.h"

#include "Gizmo.h"
#include "InputCallbacks.h"

namespace
{
    u8 objBuffer[2048];
}

EditorApp::EditorApp()
    : mWidth(0)
    , mHeight(0)
    , mOctree(Math::AABB(Math::Vector3::Zero(), Math::Vector3(50.0f, 50.0f, 50.0f)))
    , mGameObjectPool(10)
    , mCallbacks(*this)
{
    memset(mInputData.keyStates, 0, sizeof(bool) * 256);
    memset(mInputData.mouseStates, 0, sizeof(bool) * 4);
    memset(objBuffer, 0, 2048);
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

    mCallbacks.RegisterCallbacks();

    mpGizmo = new TranslateGizmo(mCamera, 1.0f, 5.0f);

    // TEMP
    GameObjectHandle handle1 = mGameObjectPool.Allocate();
    GameObjectHandle handle2 = mGameObjectPool.Allocate();

    GameObject* g1 = handle1.Get();
    GameObject* g2 = handle2.Get();
    g1->SetName("Object 1");
    g2->SetName("Object 2");

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
    SafeDelete(mpGizmo);
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

    mInputManager.Update(mInputData);

    // Destroy and re-create the entire tree
    mOctree.Destroy();
    for (u32 i=0; i < mObjects.size(); ++i)
    {
        mOctree.Insert(mObjects[i], mObjects[i].GetCollider());
    }
    mOctree.Debug_DrawTree();


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

	// Render
	mGraphicsSystem.BeginRender(Color::Black());
    
    for (auto object : mObjects)
    {
        Color col = Color::White();
        if (object.IsSelected())
        {
            col = Color::Red();
        }
        SimpleDraw::AddAABB(object.GetCollider(), col);
    }

    mpGizmo->Draw(mSelectedObjects);

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

    memset(objBuffer, 0, 2048);
    SerialWriter writer(objBuffer, 2048);

    EditorObject* editorObject = mSelectedObjects[0];
    GameObject* gameObject = editorObject->GetGameObject();
    const MetaClass* metaClass = gameObject->GetMetaClass();

    writer.WriteLengthEncodedString(metaClass->GetName());
    writer.Write(editorObject->GetHandle().GetIndex()); // used as object identifier
    writer.Write(editorObject->GetHandle().GetInstance());

    u32 offset = 0;
    gameObject->SerializeOut(objBuffer + writer.GetOffset(), 2048 - writer.GetOffset(), offset);

    //const std::vector<Component*>& components = gameObject->GetComponents();
    //writer.Write((u32)components.size());
    //for (Component* c : components)
    //{
    //    const MetaClass* compMetaClass = c->GetMetaClass();
    //    writer.WriteLengthEncodedString(compMetaClass->GetName());
    //    const MetaField* fields = compMetaClass->GetFields();
    //    const u32 numFields = compMetaClass->GetNumFields();
    //    writer.Write(numFields);
    //    for (u32 i=0; i < numFields; ++i)
    //    {
    //        const MetaField* field = &fields[i];
    //        const u32 offset = field->GetOffset();
    //        const u32 fieldSize = field->GetType()->GetSize();
    //        char* fieldData = ((char*)c) + offset;
    //        writer.WriteLengthEncodedString(field->GetName());
    //        writer.Write(field->GetType()->GetType());
    //        writer.Write(fieldSize);
    //        writer.Write(offset);
    //        writer.WriteArray(fieldData, fieldSize);
    //    }
    //}
    size = writer.GetOffset() + offset;
    return objBuffer;
}

void EditorApp::UpdateComponent(const u8* buffer, u32 buffsize)
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
            char data[2048];

            const u32 numFields = compMetaClass->GetNumFields();
            for (u32 i=0; i < numFields; ++i)
            {
                const MetaField* field = compMetaClass->GetFieldAtIndex(i);
                u32 offset = field->GetOffset();
                u32 fieldSize = field->GetType()->GetSize();
                reader.ReadArray(data, fieldSize);

                char* cdata = ((char*)c) + offset;
                memcpy(cdata, data, fieldSize);
            }
            break;
        }
    }
}

const u8* EditorApp::DiscoverGameObjects(u32& buffsize)
{
    memset(objBuffer, 0, 2048);
    SerialWriter writer(objBuffer, 2048);

    const u32 numObjects = mObjects.size();
    writer.Write(numObjects);
    for (EditorObject editorObj : mObjects)
    {
        GameObject* gameObject = editorObj.GetGameObject();
        GameObjectHandle handle = editorObj.GetHandle();

        writer.Write(handle.GetIndex());
        writer.Write(handle.GetInstance());
        writer.WriteLengthEncodedString(gameObject->GetName());
    }
    buffsize = writer.GetOffset();
    return objBuffer;
}

const u8* EditorApp::GetGameObject(u16 index, u32& buffsize)
{
    memset(objBuffer, 0, 2048);
    for (EditorObject eobj : mObjects)
    {
        GameObjectHandle handle = eobj.GetHandle();
        if (handle.GetIndex() == index)
        {
            GameObject* gameobject = eobj.GetGameObject();
            gameobject->SerializeOut(objBuffer, 2048, buffsize);
            return objBuffer;
        }
    }
    buffsize = 0;
    return nullptr;
}