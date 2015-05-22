#include "EditorApp.h"
#include "Random.h"

#include "Gizmo.h"
#include "InputCallbacks.h"

#define OBJECT_BUFF_SIZE 2048

namespace
{
    u8 objBuffer[OBJECT_BUFF_SIZE];
}

EditorApp::EditorApp()
    : mWidth(0)
    , mHeight(0)
    , mOctree(Math::AABB(Math::Vector3::Zero(), Math::Vector3(50.0f, 50.0f, 50.0f)))
    , mGameObjectPool(100)
    , mCallbacks(*this)
    , mFactory(mGameObjectPool, 100)
{
    memset(mInputData.keyStates, 0, sizeof(bool) * 256);
    memset(mInputData.mouseStates, 0, sizeof(bool) * 4);
    memset(objBuffer, 0, OBJECT_BUFF_SIZE);
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
        { "RenderService", &mRenderService }
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
        // Update the components
        GameObject* gameObject = object.GetGameObject();
        gameObject->Update(deltaTime);

        Color col = Color::White();
        if (object.IsSelected())
        {
            col = Color::Red();
        }
        SimpleDraw::AddAABB(object.GetCollider(), col);
    }

    mRenderService.Update();

    mpGizmo->Draw(mSelectedObjects);

	SimpleDraw::Render(mCamera);
	mGraphicsSystem.EndRender();
}

//----------------------------------------------------------------------------------------------------

const u8* EditorApp::GetSelectedObjectData(u32& size)
{
    // Don't display anything unless there's only a single object selected
    if (mSelectedObjects.empty() || mSelectedObjects.size() > 1)
    {
        size = 0;
        return nullptr;
    }
    EditorObject* editorObject = mSelectedObjects[0];
    return GetGameObject(editorObject->GetHandle().GetIndex(), size);;
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
            char data[OBJECT_BUFF_SIZE];

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
            // Set flag to indicate there has been a change
            c->SetIsDirty(true);
            break;
        }
    }
}

const u8* EditorApp::DiscoverGameObjects(u32& buffsize)
{
    memset(objBuffer, 0, OBJECT_BUFF_SIZE);
    SerialWriter writer(objBuffer, OBJECT_BUFF_SIZE);

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
    memset(objBuffer, 0, OBJECT_BUFF_SIZE);
    SerialWriter writer(objBuffer, OBJECT_BUFF_SIZE);

    for (EditorObject eobj : mObjects)
    {
        GameObjectHandle handle = eobj.GetHandle();
        if (handle.GetIndex() == index)
        {
            // Write out handle data
            writer.Write(handle.GetIndex());
            writer.Write(handle.GetInstance());

            GameObject* gameobject = eobj.GetGameObject();
            u32 offset = writer.GetOffset(); // Account for writing handle
            gameobject->SerializeOut(objBuffer + offset, OBJECT_BUFF_SIZE, buffsize);
            buffsize += offset;
            return objBuffer;
        }
    }
    buffsize = 0;
    return nullptr;
}

void EditorApp::SelectGameObject(u16 index)
{
    for (EditorObject& eobj : mObjects)
    {
        GameObjectHandle handle = eobj.GetHandle();
        if (handle.GetIndex() == index)
        {
            // Deselect all objects
            for (auto object : mSelectedObjects)
            {
                object->DeSelect();
            }
            mSelectedObjects.clear();

            eobj.Select(); // Set flag
            mSelectedObjects.push_back(&eobj);
        }
    }
}

void EditorApp::CreateEmptyGameObject(u16& index)
{
    GameObjectHandle handle = mFactory.Create("../Data/GameObjects/default.json");
    index = handle.GetIndex();
    mObjects.push_back(EditorObject(handle));
}

void EditorApp::RenameGameObject(u16 index, const char* name)
{
    for (EditorObject eobj : mObjects)
    {
        GameObjectHandle handle = eobj.GetHandle();
        if (handle.GetIndex() == index)
        {
            handle.Get()->SetName(name);
        }
    }
}