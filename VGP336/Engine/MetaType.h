#ifndef INCLUDED_ENGINE_METATYPE_H
#define INCLUDED_ENGINE_METATYPE_H

//====================================================================================================
// Filename:	MetaType.h
// Created by:	Tyler Staples
// Description: Class for defining a meta type.
//====================================================================================================

//====================================================================================================
// Class Declarations
//====================================================================================================

class MetaType
{
public:
    enum Type
    {
        Int,
        Float,
        Bool,
        Vector3,
        Matrix,
        // Array    // TODO
        // Pointer
        Class
    };

    MetaType(Type type, u32 size)
        : mType(type)
        , mSize(size)
    {
    }

    Type GetType() const    { return mType; }
    u32 GetSize() const     { return mSize; }

private:
    Type mType;
    u32 mSize;
};

#endif // #ifndef INCLUDED_ENGINE_METATYPE_H