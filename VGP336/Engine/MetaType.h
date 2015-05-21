#ifndef INCLUDED_ENGINE_METATYPE_H
#define INCLUDED_ENGINE_METATYPE_H

//====================================================================================================
// Filename:	MetaType.h
// Created by:	Tyler Staples
// Description: Class for defining a meta type.
//====================================================================================================

//====================================================================================================
// Forward Declarations
//====================================================================================================

namespace Json {
    class Value;
}

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
        String,
        Vector3,
        Matrix,
        AABB,
        Class,
        Array,
        Pointer,
        NumTypes
    };

    MetaType(Type type, u32 size);

    void Deserialize(const Json::Value& jsonValue, void* data) const;

    Type GetType() const    { return mType; }
    u32 GetSize() const     { return mSize; }

private:
    Type mType;
    u32 mSize;
    //bool mIsPointer;  // TODO
};

#endif // #ifndef INCLUDED_ENGINE_METATYPE_H