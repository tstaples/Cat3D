#ifndef INCLUDED_ENGINE_META_H
#define INCLUDED_ENGINE_META_H

//====================================================================================================
// Filename:	Meta.h
// Created by:	Tyler Staples
// Description: Helper classes, functions and macros for the meta system.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "MetaClass.h"
#include "MetaField.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

class MetaType;

//====================================================================================================
// Helper Functions
//====================================================================================================

template <typename DataType>
const MetaType* DeduceDataType();

//----------------------------------------------------------------------------------------------------

template <typename ClassType, typename DataType>
const MetaType* DeduceMemberDataType(DataType ClassType::*) // ::* is a pointer-to-member
{
    return DeduceDataType<DataType>();
}

//----------------------------------------------------------------------------------------------------

template <typename ClassType, typename DataType>
u32 GetFieldOffset(DataType ClassType::* field)
{
    // create a nullptr of that class type at address 0
    // get the adress of the field within the null class, giving us a byte offset
    return (u32)(void*)&(((ClassType*)nullptr)->*field);
    //return (char*)&((ClassType*)nullptr->*field) - (char*)nullptr;
}

//====================================================================================================
// Macros
//====================================================================================================

#define META_DECLARE_CLASS\
    static const MetaClass* StaticGetMetaClass();\
    virtual const MetaClass* GetMetaClass() const { return StaticGetMetaClass(); }

#define META_CLASS_BEGIN(TYPE)\
    const MetaClass* TYPE::StaticGetMetaClass()\
    {\
        typedef TYPE LocalType;\
        const char* className = #TYPE;\
        const MetaField* fields = nullptr;\
        u32 numFields = 0;

#define META_CLASS_END\
        static const MetaClass sMetaClass(className, sizeof(LocalType), fields, numFields);\
        return &sMetaClass;\
    }

#define META_FIELD_BEGIN\
    static const MetaField sFields[] = {

#define META_FIELD(FIELD, NAME)\
    MetaField(NAME, DeduceMemberDataType(&LocalType::FIELD), GetFieldOffset(&LocalType::FIELD)),

#define META_FIELD_END\
    };\
    fields = sFields;\
    numFields = sizeof(sFields) / sizeof(sFields[0]);

#endif // #ifndef INCLUDED_ENGINE_META_H