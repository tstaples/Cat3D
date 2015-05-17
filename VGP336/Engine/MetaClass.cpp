#include "Precompiled.h"

#include "MetaClass.h"
#include "MetaField.h"

MetaClass::MetaClass(const char* name, u32 classSize, const MetaField* fields, u32 numFields, CreateFunc ctor, DestroyFunc dtor)
    : MetaType(MetaType::Class, classSize)
    , mName(name)
    , mFields(fields)
    , mNumFields(numFields)
    , mConstruct(ctor)
    , mDestruct(dtor)
{
}

//----------------------------------------------------------------------------------------------------

void* MetaClass::Create() const
{
    return mConstruct();
}

//----------------------------------------------------------------------------------------------------

void MetaClass::Destroy(void* data) const
{
    mDestruct(data);
}

//----------------------------------------------------------------------------------------------------

const MetaField* MetaClass::FindField(const char* name) const
{
    for (u32 i=0; i < mNumFields; ++i)
    {
        const MetaField* field = &mFields[i];
        if (strcmp(name, field->GetName()) == 0)
        {
            return field;
        }
    }
    return nullptr;
}

//----------------------------------------------------------------------------------------------------

const MetaField* MetaClass::GetFieldAtIndex(u32 index) const
{
    ASSERT(index < mNumFields, "[MetaClass] Invalid field index for MetaClass: %s", mName);
    return (mFields + index);
}