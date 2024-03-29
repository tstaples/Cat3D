#include "Precompiled.h"
#include "GameObject.h"

#include "Component.h"
#include "GameWorld.h"
#include "SerialWriter.h"
#include "SerialReader.h"

META_CLASS_BEGIN(GameObject)
META_FIELD_BEGIN
    META_FIELD(mName, "Name")
META_FIELD_END
META_CLASS_END

namespace
{
    bool GetComponentByName(const GameObject::Components& components, const char* name, Component* component)
    {
        for (Component* c : components)
        {
            const MetaClass* compMetaClass = c->GetMetaClass();
            if (strcmp(name, compMetaClass->GetName()) == 0)
            {
                component = c;
                return true;
            }
        }
        component = nullptr;
        return false;
    }
}

GameWorld* GameObject::spWorld = nullptr;

GameObject::GameObject()
    : mName("Unknown")
    , mToBeDestroyed(false)
{
    memset(mServiceSubscriptions, 0, kNumServices);
}

//----------------------------------------------------------------------------------------------------

GameObject::GameObject(const char* name)
    : mName(name)
    , mToBeDestroyed(false)
{
}

//----------------------------------------------------------------------------------------------------

GameObject::~GameObject()
{
}

//----------------------------------------------------------------------------------------------------

void GameObject::Update(f32 deltaTime)
{
    for (Component* c : mComponents)
    {
        c->Update(deltaTime);

        // Flag unchecked in last service to run for now until I can find a better method.
        // Reset flag as anything that relies on it will have been dealt with already
        //c->SetIsDirty(false);
    }
}

//----------------------------------------------------------------------------------------------------

void GameObject::AddService(u16 id)
{
    ASSERT(id < kNumServices, "[GameObject] serivce ID out of bounds");
    ASSERT(!mServiceSubscriptions[id], "[GameObject] already subscribed to service id: %u", id);
    mServiceSubscriptions[id] = true;
}

//----------------------------------------------------------------------------------------------------

void GameObject::RemoveService(u16 id)
{
    ASSERT(id < kNumServices, "[GameObject] serivce ID out of bounds");
    ASSERT(mServiceSubscriptions[id], "[GameObject] not subscribed to service id: %u", id);
    mServiceSubscriptions[id] = false;
}

//----------------------------------------------------------------------------------------------------

bool GameObject::HasService(u16 id) const
{
    ASSERT(id < kNumServices, "[GameObject] serivce ID out of bounds");
    return mServiceSubscriptions[id];
}

//----------------------------------------------------------------------------------------------------

void GameObject::AddComponent(Component* component)
{
    // Inject ourself into the component as we now own it.
    component->mpGameObject = this;
    mComponents.push_back(component);
}

//----------------------------------------------------------------------------------------------------

bool GameObject::Serialize(SerialWriter& writer)
{
    // Write the gameobject's name and number of componenets
    writer.WriteLengthEncodedString(mName.c_str());
    writer.Write((u32)mComponents.size());
    for (Component* c : mComponents)
    {
        // Probably don't need to write this, but may come in handy
        const MetaClass* compMetaClass = c->GetMetaClass();
        writer.WriteLengthEncodedString(compMetaClass->GetName());

        // Write how many members this component has
        const u32 numFields = compMetaClass->GetNumFields();
        writer.Write(numFields);
        for (u32 i=0; i < numFields; ++i)
        {
            const MetaField* field = compMetaClass->GetFieldAtIndex(i);
            const MetaType* metaType = field->GetType();
            const u32 offset = field->GetOffset();
            const u32 fieldSize = field->GetType()->GetSize();
            void* fieldData = ((char*)c) + offset;

            // TODO: The metadata is only needed for the editor side, so we shouldn't be
            // writing it in the game object.

            // Write the metadata for each field then the data itself
            writer.WriteLengthEncodedString(field->GetName());
            writer.Write(field->GetType()->GetType());
            writer.Write(fieldSize);
            writer.Write(offset);
            metaType->Serialize(fieldData, writer);
        }
    }
    return true;
}

//----------------------------------------------------------------------------------------------------

bool GameObject::Deserialize(SerialReader& reader)
{
    mName = reader.ReadLengthEncodedString();
    const u32 numComponents = reader.Read<u32>();
    for (u32 i=0; i < numComponents; ++i)
    {
        bool isNewComponent = false;

        // Look up component by name
        Component* component = nullptr;
        std::string compMetaClassName = reader.ReadLengthEncodedString();
        if (!GetComponentByName(mComponents, compMetaClassName.c_str(), component))
        {
            // If it doesn't exist, create it
            const MetaClass* compMetaClass = MetaDB::GetMetaClass(compMetaClassName.c_str());
            component = static_cast<Component*>(compMetaClass->Create());
            component->mpGameObject = this;
            isNewComponent = true;
        }

        // Read in the component fields
        const u32 numFields = reader.Read<u32>();
        for (u32 j=0; j < numFields; ++j)
        {
            const MetaField* metaField = component->GetMetaClass()->GetFieldAtIndex(j);
            const MetaType* metaType = metaField->GetType();

            std::string fieldName = reader.ReadLengthEncodedString();
            MetaType::Type fieldType = reader.Read<MetaType::Type>();
            const u32 fieldSize = reader.Read<u32>();
            const u32 fieldOffset = reader.Read<u32>();
            
            // May as well make use of the metadata
            ASSERT(fieldName == metaField->GetName(), "[GameObject] Deserialization error: field names do not match");
            ASSERT(fieldType == metaField->GetType()->GetType(), "[GameObject] Deserialization error: field types do not match");
            ASSERT(fieldOffset == metaField->GetOffset(), "[GameObject] Deserialization error: field offsets do not match");
            ASSERT(fieldSize == metaField->GetType()->GetSize(), "[GameObject] Deserialization error: field sizes do not match");

            // Get a pointer to the field and deserialize the data into it
            void* fieldData = ((u8*)component) + fieldOffset;
            metaType->Deserialize(reader, fieldData);
        }

        if (isNewComponent)
        {
            AddComponent(component);
        }
    }
    return true;
}

//----------------------------------------------------------------------------------------------------

bool GameObject::HasComponent(const char* componentName) const
{
    for (Component* c : mComponents)
    {
        const MetaClass* compMetaClass = c->GetMetaClass();
        if (strcmp(componentName, compMetaClass->GetName()) == 0)
        {
            return true;
        }
    }
    return false;
}

//----------------------------------------------------------------------------------------------------

GameWorld* GameObject::GetWorld() const
{
    return spWorld;
}