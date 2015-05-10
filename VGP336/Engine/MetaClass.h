#ifndef INCLUDED_ENGINE_METACLASS_H
#define INCLUDED_ENGINE_METACLASS_H

//====================================================================================================
// Filename:	MetaClass.h
// Created by:	Tyler Staples
// Description: Class for storing meta info about a class.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "MetaType.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

class MetaField;

//====================================================================================================
// Class Declarations
//====================================================================================================

class MetaClass : public MetaType
{
public:
    // Constructor
    // @param name: name of the class.
    // @param classSize: sizeof the class.
    // @param fields: pointer to a static array of fields that represent the class members.
    // @param numFields: number of fields in the array.
    MetaClass(const char* name, u32 classSize, const MetaField* fields, u32 numFields)
        : MetaType(MetaType::Class, classSize)
        , mName(name)
        , mFields(fields)
        , mNumFields(numFields)
    {
    }

    const char* GetName() const { return mName; }

private:
    // Will point to literal constant, so shallow copy is fine.
    const char* mName;

    const MetaField* mFields;
    const u32 mNumFields;
};

#endif // #ifndef INCLUDED_ENGINE_METACLASS_H