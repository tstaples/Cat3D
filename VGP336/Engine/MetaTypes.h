#ifndef INCLUDED_ENGINE_METATYPES_H
#define INCLUDED_ENGINE_METATYPES_H

//====================================================================================================
// Filename:	MetaTypes.h
// Created by:	Tyler Staples
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Hash.h"

//====================================================================================================
// Namespace
//====================================================================================================

namespace Meta
{

enum Type
{
// Append 'Type' to the enum value
#define DEFTYPE(exp) exp##Type,
#include "MetaTypeDefines.tup"
#undef DEFTYPE
    TypeMax // # of types in this enum
};

static const char* name[] =
{
#define DEFTYPE(exp) #exp,
#include "MetaTypeDefines.tup"
#undef DEFTYPE
};

static const u64 Hash[] = 
{
#define DEFTYPE(exp) GetHash(#exp),
#include "MetaTypeDefines.tup"
#undef DEFTYPE
};

inline Type GetEnumValue(const char* str)
{
    for (u32 i=0; i < TypeMax; ++i)
    {
        if (strcmp(str, name[i]) == 0)
        {
            return Type(i);
        }
    }
    return TypeMax;
}

} // namespace Meta

#endif // #ifndef INCLUDED_ENGINE_METATYPES_H