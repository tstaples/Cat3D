#ifndef INCLUDED_ENGINE_ID_H
#define INCLUDED_ENGINE_ID_H

//====================================================================================================
// Filename:	ID.h
// Created by:	Tyler Staples
// Description: Class for a meta type ID.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "MetaTypes.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

// Meta type (1 byte)
// Index (2 bytes)
// Instance - 

class ID
{
    friend bool operator==(const ID& lhs, const ID& rhs) { return (lhs.blob == rhs.blob); }
    friend bool operator!=(const ID& lhs, const ID& rhs) { return !(lhs == rhs); }
    friend bool operator<(const ID& lhs, const ID& rhs)  { return (lhs.blob < rhs.blob); }
    friend bool operator>(const ID& lhs, const ID& rhs)  { return (lhs.blob > rhs.blob); }

public:
    ID();
    ID(Meta::Type type, u8 instance, u16 index);

    Meta::Type GetType() const  { return ((Meta::Type)flags.type); }
    u8 GetInstance() const      { return flags.instance; }
    u16 GetIndex() const        { return flags.index; }

    bool IsValid() const        { return (blob != u32_MAX); }
    void Invalidate()           { blob = u32_MAX; }

private:
    union
    {
        struct
        {
            u8 type;
            u8 instance;
            u16 index;
        } flags;

        u32 blob;
    };
};

#endif // #ifndef INCLUDED_ENGINE_ID_H

/*
bool IsLittleEndian()
{
    union Temp
    {
        int i;
        char c;
    };

    Temp temp;
    temp.i = 0x000000ff;
    return (temp.c == 0x0f);
}
*/