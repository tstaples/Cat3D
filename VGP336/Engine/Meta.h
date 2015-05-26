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
#include "MetaDB.h"
#include "MetaDependency.h"
#include "MetaField.h"
#include "MetaRegistration.h"

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
}

//====================================================================================================
// Macros
//====================================================================================================

// Must be used in the class declaration
#define META_DECLARE_CLASS\
    static const MetaClass* StaticGetMetaClass();\
    virtual const MetaClass* GetMetaClass() const { return StaticGetMetaClass(); }

// Used in the class declaration cpp.
// Because of the method being used to allow the creation/deletion of instances
// of an object described by the meta system, they must provide a default constructor.
// This could be avoided by having Create() take a void* param and casting it to the 
// object type, but it isn't very typesafe, or necessary for the time being.
#define META_CLASS_BEGIN(TYPE)\
    namespace {\
        void* Create() { return new TYPE(); }\
        void Destroy(void* data) { delete static_cast<TYPE*>(data); }\
    }\
    const MetaClass* TYPE::StaticGetMetaClass()\
    {\
        /* typedef to allow usage of the type in below macros. */\
        typedef TYPE LocalType;\
        const char* className = #TYPE;\
        const MetaField* fields = nullptr;\
        u32 numFields = 0;\
        const MetaDependency* dependencies = nullptr;\
        u32 numDependencies = 0;

#define META_FIELD_BEGIN\
        static const MetaField sFields[] = {

#define META_FIELD(FIELD, NAME)\
        MetaField(NAME, DeduceMemberDataType(&LocalType::FIELD), GetFieldOffset(&LocalType::FIELD)),

#define META_FIELD_END\
        };\
        fields = sFields;\
        numFields = STATIC_ARRAY_SIZE(sFields);

#define META_DEPENDENCIES_BEGIN\
        static const MetaDependency sDependencies[] = {

#define META_DEPENDENCY(CLASS_NAME)\
        MetaDependency(CLASS_NAME),

#define META_DEPENDENCIES_END\
        };\
        dependencies = sDependencies;\
        numDependencies = STATIC_ARRAY_SIZE(sDependencies);

#define META_CLASS_END\
        bool ispointer = std::is_pointer<LocalType>::value;\
        static const MetaClass sMetaClass(className, sizeof(LocalType), fields, numFields, dependencies, numDependencies, ispointer, Create, Destroy);\
        return &sMetaClass;\
    }

#endif // #ifndef INCLUDED_ENGINE_META_H