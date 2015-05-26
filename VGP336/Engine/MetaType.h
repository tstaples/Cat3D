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
class SerialWriter;
class SerialReader;

//====================================================================================================
// Class Declarations
//====================================================================================================

class MetaType
{
public:
    enum Type
    {
        Int,
        UInt,
        Short,
        UShort,
        Char,
        UChar,
        Float,
        Double,
        Bool,
        String,
        WString,
        Vector3,
        Matrix,
        AABB,
        Class,
        Array,
        Pointer,
        NumTypes
    };

    MetaType(Type type, u32 size, bool ispointer, bool mIsArray, u32 arrsize=0);

    // TODO: Serialize to JSON

    // Writes the the data into a binary stream.
    // @param data: data to serialize.
    // @param write: binary stream to write to.
    // Note: length encodes all strings.
    void Serialize(void* data, SerialWriter& writer) const;

    // Writes the JSON data to an output buffer.
    // @param jsonValue: Jsoncpp value object containing the data to write.
    // @param dst: destination to write to.
    void Deserialize(const Json::Value& jsonValue, void* dst) const;

    // Writes the binary stream data to the destination buffer.
    // @param reader: binary stream to read from.
    // @param dst: destination to write to.
    // Note: reads all strings assuming they're length encoded. 
    // This length byte is not written to the dst.
    void Deserialize(SerialReader& reader, void* dst) const;

    Type GetType() const    { return mType; }
    u32 GetSize() const     { return mSize; }

private:
    Type mType;
    u32 mSize;
    bool mIsPointer;  // TODO
    bool mIsArray;
    u32 mArrLength;
};

#endif // #ifndef INCLUDED_ENGINE_METATYPE_H