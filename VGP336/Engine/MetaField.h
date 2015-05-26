#ifndef INCLUDED_ENGINE_METAFIELD_H
#define INCLUDED_ENGINE_METAFIELD_H

//====================================================================================================
// Filename:	MetaField.h
// Created by:	Tyler Staples
// Description: Class for storing meta info about a class member.
//====================================================================================================

//====================================================================================================
// Forward Declarations
//====================================================================================================

class MetaType;

//====================================================================================================
// Class Declarations
//====================================================================================================

class MetaField
{
public:
    // Constructor
    // @param name: name of the member/field.
    // @param type: field's data type.
    // @param offset: address offset of the member from the start of the class.
    MetaField(const char* name, const MetaType* type, u32 offset)
        : mName(name)
        , mType(type)
        , mOffset(offset)
    {
    }

    const char* GetName() const     { return mName; }
    const MetaType* GetType() const { return mType; }
    u32 GetOffset() const           { return mOffset; }

private:
    const char* mName;
    const MetaType* mType;
    u32 mOffset;
};

#endif // #ifndef INCLUDED_ENGINE_METAFIELD_H