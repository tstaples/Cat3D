#include "Precompiled.h"
#include "EditorCommands.h"
#include "EditorApp.h"

#include <Engine.h>

#define OBJECT_BUFF_SIZE 2048

namespace
{
    u8 objBuffer[OBJECT_BUFF_SIZE];
}

EditorCommands::EditorCommands(EditorApp& app)
    : mApp(app)
{
    memset(objBuffer, 0, OBJECT_BUFF_SIZE);
}

//----------------------------------------------------------------------------------------------------

const u8* EditorCommands::GetSelectedObjectData(u32& size)
{
    std::vector<EditorObject*>& selectedObjects = mApp.mSelectedObjects;

    // Don't display anything unless there's only a single object selected
    if (selectedObjects.empty() || selectedObjects.size() > 1)
    {
        size = 0;
        return nullptr;
    }
    EditorObject* editorObject = selectedObjects[0];
    return GetGameObject(editorObject->GetHandle().GetIndex(), size);;
}

//----------------------------------------------------------------------------------------------------

s32 EditorCommands::UpdateComponent(const u8* buffer, u32 buffsize)
{
    s32 rc = 1;
    SerialReader reader((u8*)buffer, buffsize);

    // Get handle data to find corresponding gameobject
    u16 index = reader.Read<u16>();
    u16 instance = reader.Read<u16>();
    GameObjectHandle handle(instance, index);
    GameObject* gameObject = mApp.mGameObjectPool.Get(handle);
    if (gameObject == nullptr)
        return rc;

    std::string compName = reader.ReadLengthEncodedString();
    const std::vector<Component*>& components = gameObject->GetComponents();
    for (Component* c : components)
    {
        // Find the component by name
        const MetaClass* compMetaClass = c->GetMetaClass();
        if (compName.compare(compMetaClass->GetName()) == 0)
        {
            char data[OBJECT_BUFF_SIZE] = { 0 };

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
            rc = 0; // success
            break;
        }
    }
    return rc;
}

//----------------------------------------------------------------------------------------------------

const u8* EditorCommands::DiscoverGameObjects(u32& buffsize)
{
    memset(objBuffer, 0, OBJECT_BUFF_SIZE);
    SerialWriter writer(objBuffer, OBJECT_BUFF_SIZE);

    const u32 numObjects = mApp.mObjects.size();
    writer.Write(numObjects);
    for (EditorObject editorObj : mApp.mObjects)
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

//----------------------------------------------------------------------------------------------------

const u8* EditorCommands::GetGameObject(u16 index, u32& buffsize)
{
    memset(objBuffer, 0, OBJECT_BUFF_SIZE);
    SerialWriter writer(objBuffer, OBJECT_BUFF_SIZE);

    for (EditorObject eobj : mApp.mObjects)
    {
        GameObjectHandle handle = eobj.GetHandle();
        if (handle.GetIndex() == index)
        {
            // Write out handle data
            writer.Write(handle.GetIndex());
            writer.Write(handle.GetInstance());

            GameObject* gameobject = eobj.GetGameObject();
            u32 offset = writer.GetOffset(); // Account for writing handle
            gameobject->Serialize(objBuffer + offset, OBJECT_BUFF_SIZE, buffsize);
            buffsize += offset;
            return objBuffer;
        }
    }
    buffsize = 0;
    return nullptr;
}

//----------------------------------------------------------------------------------------------------

void EditorCommands::SelectGameObject(u16 index)
{
    std::vector<EditorObject*>& selectedObjects = mApp.mSelectedObjects;
    for (EditorObject& eobj :mApp. mObjects)
    {
        GameObjectHandle handle = eobj.GetHandle();
        if (handle.GetIndex() == index)
        {
            // Deselect all objects
            for (auto object : selectedObjects)
            {
                object->DeSelect();
            }
            selectedObjects.clear();

            eobj.Select(); // Set flag
            selectedObjects.push_back(&eobj);
        }
    }
}

//----------------------------------------------------------------------------------------------------

void EditorCommands::CreateEmptyGameObject(u16& index)
{
    GameObjectHandle handle = mApp.mFactory.Create("../Data/GameObjects/default.json");
    index = handle.GetIndex();
    mApp.mObjects.push_back(EditorObject(handle));
}

//----------------------------------------------------------------------------------------------------

void EditorCommands::RenameGameObject(u16 index, const char* name)
{
    for (EditorObject eobj : mApp.mObjects)
    {
        GameObjectHandle handle = eobj.GetHandle();
        if (handle.GetIndex() == index)
        {
            handle.Get()->SetName(name);
        }
    }
}