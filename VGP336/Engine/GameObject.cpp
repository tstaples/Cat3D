#include "Precompiled.h"
#include "GameObject.h"

#include "Component.h"
#include "SerialWriter.h"
#include "SerialReader.h"

META_CLASS_BEGIN(GameObject)
META_FIELD_BEGIN
    META_FIELD(mName, "Name")
META_FIELD_END
META_CLASS_END

GameObject::GameObject()
    : mName("Unknown")
{
}

//----------------------------------------------------------------------------------------------------

GameObject::GameObject(const char* name)
    : mName(name)
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
        // Reset flag as anything that relies on it will have been dealt with
        // already.
        c->SetIsDirty(false);
    }
}

//----------------------------------------------------------------------------------------------------

void GameObject::AddComponent(Component* component)
{
    // Inject ourself into the component as we now own it.
    component->mpGameObject = this;
    mComponents.push_back(component);
}

//----------------------------------------------------------------------------------------------------

void GameObject::SerializeOut(u8* buffer, u32 size, u32& offset)
{
    SerialWriter writer(buffer, size);

    // Write the gameobject's name and number of componenets
    writer.WriteLengthEncodedString(mName.c_str());
    writer.Write((u32)mComponents.size());
    for (Component* c : mComponents)
    {
        // Probably don't need to write this, but may come in handy
        const MetaClass* compMetaClass = c->GetMetaClass();
        writer.WriteLengthEncodedString(compMetaClass->GetName());

        const u32 numFields = compMetaClass->GetNumFields();
        writer.Write(numFields);
        for (u32 i=0; i < numFields; ++i)
        {
            const MetaField* field = compMetaClass->GetFieldAtIndex(i);
            const u32 offset = field->GetOffset();
            const u32 fieldSize = field->GetType()->GetSize();
            char* fieldData = ((char*)c) + offset;

            // Write the metadata for each field then the data itself
            writer.WriteLengthEncodedString(field->GetName());
            writer.Write(field->GetType()->GetType());
            writer.Write(fieldSize);
            writer.Write(offset);
            writer.WriteArray(fieldData, fieldSize);
        }
    }
    offset = writer.GetOffset();
}

//----------------------------------------------------------------------------------------------------

void GameObject::SerializeIn(const u8* buffer, u32 size)
{
    // TODO

    //SerialReader reader(buffer, size);
    //
    //mName = reader.ReadLengthEncodedString();
    //const u32 numComponents = reader.Read<u32>();
    //ASSERT(numComponents == mComponents.size(), "[GameObject] Error serializing into object: number of components does not match");

    //std::string compName = reader.ReadLengthEncodedString();
    //for (Component* c : mComponents)
    //{
    //    // Find the component by name
    //    const MetaClass* compMetaClass = c->GetMetaClass();
    //    if (compName.compare(compMetaClass->GetName()) == 0)
    //    {
    //        char data[2048];

    //        const MetaField* fields = compMetaClass->GetFields();
    //        const u32 numFields = compMetaClass->GetNumFields();
    //        for (u32 i=0; i < numFields; ++i)
    //        {
    //            u32 offset = fields[i].GetOffset();
    //            u32 fieldSize = fields[i].GetType()->GetSize();
    //            reader.ReadArray(data, fieldSize);

    //            char* cdata = ((char*)c) + offset;
    //            memcpy(cdata, data, fieldSize);
    //        }
    //        break;
    //    }
    //}
}