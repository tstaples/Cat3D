#include "Precompiled.h"
#include "EditorCommands.h"
#include "EditorApp.h"

#include <Engine.h>

#define OBJECT_BUFF_SIZE 2048

struct Handle
{
    u16 instance;
    u16 index;

    Handle(){}

    Handle(const GameObjectHandle& goH)
    {
        instance = goH.GetInstance();
        index = goH.GetIndex();
    }

    Handle& operator=(const GameObjectHandle& rhs)
    {
        instance = rhs.GetInstance();
        index = rhs.GetIndex();
        return *this;
    }

    operator GameObjectHandle()
    {
        return GameObjectHandle(instance, index);
    }

    bool operator==(const GameObjectHandle& rhs)
    {
        return (index == rhs.GetIndex() && instance == rhs.GetInstance());
    }
    bool operator!=(const GameObjectHandle& rhs) 
    { 
        return !(*this == rhs); 
    }
};

namespace
{
    static std::string lastErrorMsg;
    void SetError(const char* format, ...)
    {
        char buff[8196];
	    va_list va;
	    va_start(va, format);
	    s32 written = (s32)vsnprintf(buff, sizeof(buff), format, va);
	    va_end(va);

        buff[written] = '\0';
	    ::OutputDebugStringA(buff);
	    ::OutputDebugStringA("\n");
        lastErrorMsg = buff;
    }

    GameObjectHandle GetHandleFromNative(Handle handle, EditorObjects& objects)
    {
        for (EditorObject eobj : objects)
        {
            GameObjectHandle gohandle = eobj.GetHandle();
            if (handle == gohandle)
            {
                return gohandle;
            }
        }
        return GameObjectHandle();
    }
}


//----------------------------------------------------------------------------------------------------

EditorCommands::EditorCommands(EditorApp& app)
    : mApp(app)
{
}

//----------------------------------------------------------------------------------------------------

const char* EditorCommands::GetLastError()
{
    return lastErrorMsg.c_str();
}

//----------------------------------------------------------------------------------------------------

bool EditorCommands::GetSelectedObjectData(u8* dst, u32 size, u32& bytesWritten)
{
    std::vector<EditorObject*>& selectedObjects = mApp.mSelectedObjects;

    // Don't display anything unless there's only a single object selected
    if (selectedObjects.size() == 1)
    {
        EditorObject* editorObject = selectedObjects.front();
        Handle handle(editorObject->GetHandle());
        return GetGameObject(handle, dst, size, bytesWritten);
    }
    SetError("[EditorCommands] Single object not selected");
    bytesWritten = 0;
    return false;
}

//----------------------------------------------------------------------------------------------------

bool EditorCommands::UpdateComponent(const u8* buffer, u32 buffsize)
{
    SerialReader reader((u8*)buffer, buffsize);

    // Get handle data to find corresponding gameobject
    u16 instance = reader.Read<u16>();
    u16 index = reader.Read<u16>();
    GameObjectHandle handle(instance, index);
    GameObject* gameObject = mApp.mGameWorld.mGameObjectPool.Get(handle);
    if (gameObject == nullptr)
    {
        SetError("[EditorCommands] Could not get gameObject with instance: %u index: %u", instance, index);
        return false;
    }

    std::string compName = reader.ReadLengthEncodedString();
    const std::vector<Component*>& components = gameObject->GetComponents();
    for (Component* c : components)
    {
        // Find the component by name
        const MetaClass* compMetaClass = c->GetMetaClass();
        if (compName.compare(compMetaClass->GetName()) == 0)
        {
            const u32 numFields = compMetaClass->GetNumFields();
            for (u32 i=0; i < numFields; ++i)
            {
                const MetaField* field = compMetaClass->GetFieldAtIndex(i);
                const MetaType* metaType = field->GetType();

                // Deserialize the data into the member
                u32 offset = field->GetOffset();
                u8* cdata = ((u8*)c) + offset;
                metaType->Deserialize(reader, cdata);
            }
            // Set flag to indicate there has been a change
            c->SetIsDirty(true);
            return true;
        }
    }
    SetError("[EditorCommands] Failed to find component with name %s", compName.c_str());
    return false;
}

//----------------------------------------------------------------------------------------------------

// TODO: store data in struct instead?
bool EditorCommands::DiscoverGameObjects(u8* dst, u32 size, u32& bytesWritten)
{
    SerialWriter writer(dst, size);

    const u32 numObjects = mApp.mObjects.size();
    writer.Write(numObjects);
    for (EditorObject editorObj : mApp.mObjects)
    {
        GameObject* gameObject = editorObj.GetGameObject();
        GameObjectHandle handle = editorObj.GetHandle();

        writer.Write(handle.GetInstance());
        writer.Write(handle.GetIndex());
        writer.WriteLengthEncodedString(gameObject->GetName());
    }
    bytesWritten = writer.GetOffset();
    return true;
}

//----------------------------------------------------------------------------------------------------

bool EditorCommands::GetGameObject(Handle handle, u8* dst, u32 size, u32& bytesWritten)
{
    SerialWriter writer(dst, size);

    for (EditorObject eobj : mApp.mObjects)
    {
        GameObjectHandle gohandle = eobj.GetHandle();
        if (handle == gohandle)
        {
            // Write out handle data
            writer.Write(gohandle.GetInstance());
            writer.Write(gohandle.GetIndex());

            GameObject* gameobject = eobj.GetGameObject();
            gameobject->Serialize(writer);
            bytesWritten += writer.GetOffset();
            return true;
        }
    }
    SetError("[EditorCommands] Could not get gameObject with instance: %u index: %u", 
        handle.instance, handle.index);
    bytesWritten = 0;
    return false;
}

//----------------------------------------------------------------------------------------------------

bool EditorCommands::SelectGameObject(Handle handle)
{
    std::vector<EditorObject*>& selectedObjects = mApp.mSelectedObjects;
    for (EditorObject& eobj : mApp.mObjects)
    {
        GameObjectHandle gohandle = eobj.GetHandle();
        if (handle == gohandle)
        {
            // Deselect all objects
            for (auto object : selectedObjects)
            {
                object->DeSelect();
            }
            selectedObjects.clear();

            eobj.Select(); // Set flag
            selectedObjects.push_back(&eobj);
            return true;
        }
    }
    SetError("[EditorCommands] Could not find gameObject with instance: %u index: %u", 
        handle.instance, handle.index);
    return false;
}

//----------------------------------------------------------------------------------------------------

bool EditorCommands::CreateGameObjectFromTemplate(const char* templateFile, Handle& handle)
{
    GameObjectHandle gohandle = mApp.mGameWorld.CreateGameObject(templateFile, 
        Math::Vector3::Zero(), Math::Quaternion::Identity());
    if (gohandle.IsValid())
    {
        handle = gohandle;
        mApp.mObjects.push_back(EditorObject(gohandle));
        return true;
    }
    SetError("[EditorCommands] Failed to create GameObject");
    return false;
}

//----------------------------------------------------------------------------------------------------

bool EditorCommands::DestroyGameObject(Handle handle)
{
    GameObjectHandle gohandle = handle;
    if (gohandle.IsValid())
    {
        if (mApp.mGameWorld.mFactory.Destroy(gohandle))
        {
            // Remove the object from the editor's list
            EditorObjects::iterator it = mApp.mObjects.begin();
            for (it; it != mApp.mObjects.end(); ++it)
            {
                if (it->GetHandle() == handle)
                {
                    mApp.mObjects.erase(it);
                    break;
                }
            }
            // Deleted object would have been the one selected
            mApp.mSelectedObjects.clear();
            return true;
        }
    }
    return false;
}

//----------------------------------------------------------------------------------------------------

bool EditorCommands::RenameGameObject(Handle handle, const char* name)
{
    ASSERT(name != nullptr, "[EditorCommands] Renaming error: name is uninitialized");

    GameObjectHandle gohandle = GetHandleFromNative(handle, mApp.mObjects);
    if (gohandle.IsValid())
    {
        // Update the GameObject's name
        gohandle.Get()->SetName(name);
        return true;
    }
    SetError("[EditorCommands] Could not find gameObject with instance: %u index: %u", 
        handle.instance, handle.index);
    return false;
}

//----------------------------------------------------------------------------------------------------

bool EditorCommands::AddComponent(Handle handle, const char* componentName)
{
    bool success = false;
    GameObjectHandle gohandle = GetHandleFromNative(handle, mApp.mObjects);
    if (gohandle.IsValid())
    { 
        // only care about component dependencies
        // if we're adding a component that requires a service, but the GO doesn't have everything
        // the service needs, then don't subscribe.
        success = mApp.mGameWorld.mFactory.AddComponent(handle, componentName);
    }
    return success;
}

//----------------------------------------------------------------------------------------------------

bool EditorCommands::RemoveComponent(Handle handle, const char* componentName)
{
    GameObjectHandle gohandle = GetHandleFromNative(handle, mApp.mObjects);
    if (gohandle.IsValid())
    {
        return mApp.mGameWorld.mFactory.RemoveComponent(gohandle, componentName);
    }
    return false;
}

//----------------------------------------------------------------------------------------------------

bool EditorCommands::GetMetaData(u8* dst, u32 size, u32& bytesWritten)
{
    return MetaDB::ExportMetaData(dst, size, bytesWritten);
}

//----------------------------------------------------------------------------------------------------

bool EditorCommands::NewLevel(const char* filename)
{
    if (mApp.mGameWorld.NewLevel(filename))
    {
        mApp.mObjects.clear();
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------

bool EditorCommands::SaveLevel(const char* filename)
{
    return mApp.mGameWorld.SaveLevel(filename);
}

//----------------------------------------------------------------------------------------------------

bool EditorCommands::LoadLevel(const char* filename)
{
    if (mApp.mGameWorld.LoadLevel(filename))
    {
        // Clear any existing items and discover the new ones
        mApp.mObjects.clear();
        for (GameObjectHandle handle : mApp.mGameWorld.mUpdateList)
        {
            mApp.mObjects.push_back(EditorObject(handle));
        }
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------

bool EditorCommands::StartGame()
{
    GameWorld& gameWorld = mApp.mGameWorld;
    bool saved = gameWorld.SaveLevelToMemory();
    ASSERT(saved, "[EditorCommands] Failed to save level to memory");
    mApp.mIsGameRunning = true;
    mApp.UpdateDelegate = DELEGATE(&EditorApp::OnGameUpdate, &mApp);
    return saved;
}

//----------------------------------------------------------------------------------------------------

bool EditorCommands::PauseGame()
{
    mApp.mIsGameRunning = false;
    return true;
}

//----------------------------------------------------------------------------------------------------

bool EditorCommands::StopGame()
{
    // Reload current scene
    GameWorld& gameWorld = mApp.mGameWorld;
    bool loaded = gameWorld.ReLoadCurrentLevel();
    ASSERT(loaded, "[EditorCommands] Failed to load level from memory");
    mApp.mIsGameRunning = false;
    mApp.UpdateDelegate = DELEGATE(&EditorApp::OnEditorUpdate, &mApp);

    // Update the editor's objects
    mApp.mObjects.clear();
    for (GameObjectHandle handle : mApp.mGameWorld.mUpdateList)
    {
        mApp.mObjects.push_back(EditorObject(handle));
    }

    return loaded;
}