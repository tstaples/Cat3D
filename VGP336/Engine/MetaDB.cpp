#include "Precompiled.h"

#include "MetaDB.h"
#include "Meta.h"
#include "Hash.h"
#include "SerialWriter.h"

namespace
{
    typedef std::map<u64, const MetaClass*> MetaClassMap;
    MetaClassMap sMetaClasses;
}

//----------------------------------------------------------------------------------------------------

void MetaDB::Register(const MetaClass* metaClass)
{
    const u64 key = GetHash(metaClass->GetName());
    // This assert should never fire, but just to be sure
    ASSERT(sMetaClasses.find(key) == sMetaClasses.end(), "[MetaDB] Register error: MetaClass %s is already registered", metaClass->GetName());
    sMetaClasses.insert(std::make_pair(key, metaClass));
}

//----------------------------------------------------------------------------------------------------

const MetaClass* MetaDB::GetMetaClass(const char* className)
{
    const MetaClass* metaClass = nullptr;
    u64 key = GetHash(className);
    MetaClassMap::iterator it = sMetaClasses.find(key);

    // Note: ensure MetaDB::Register is being called if this assert hits
    ASSERT(it != sMetaClasses.end(), "[MetaDB] Meta class for %s not found.", className);
    if (it != sMetaClasses.end())
    {
        metaClass = it->second;
    }
    return metaClass;
}

//----------------------------------------------------------------------------------------------------

bool MetaDB::ExportMetaData(u8* dst, u32 dstSize, u32& offset)
{
    SerialWriter writer(dst, dstSize);
    writer.Write(sMetaClasses.size());

    for (auto& pair : sMetaClasses)
    {
        // meta class name
        // number of fields
        // fields...
        //  name
        //  type
        //  size
        //  offset
        // number of deps
        // deps...
        const MetaClass* metaClass = pair.second;
        writer.WriteLengthEncodedString(metaClass->GetName());

        const u32 numFields = metaClass->GetNumFields();
        writer.Write(numFields);
        for (u32 i=0; i < numFields; ++i)
        {
            const MetaField* metaField = metaClass->GetFieldAtIndex(i);
            const MetaType* metaType = metaField->GetType();
            writer.WriteLengthEncodedString(metaField->GetName());
            writer.Write(metaType->GetType());
            writer.Write(metaType->GetSize());
            writer.Write(metaField->GetOffset());
        }

        const u32 numDeps = metaClass->GetNumDependencies();
        writer.Write(numDeps);
        for (u32 i=0; i < numDeps; ++i)
        {
            const MetaDependency* metaDependency = metaClass->GetDependencyAtIndex(i);
            writer.WriteLengthEncodedString(metaDependency->GetName());
            writer.WriteLengthEncodedString(metaDependency->GetTag());
        }
    }
    offset = writer.GetOffset();
    return true;
}