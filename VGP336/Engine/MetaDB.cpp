#include "Precompiled.h"

#include "MetaDB.h"
#include "MetaClass.h"
#include "Hash.h"

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