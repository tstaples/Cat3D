#ifndef INCLUDED_ENGINE_MEMHANDLE_H
#define INCLUDED_ENGINE_MEMHANDLE_H

//====================================================================================================
// Filename:	MemHandle.h
// Created by:	Tyler Staples
// Description: Class for a handle to a memory block.
//====================================================================================================

//====================================================================================================
// Class Declarations
//====================================================================================================

class MemHandle
{
    friend bool operator==(MemHandle lhs, MemHandle rhs) { return (lhs.mIndex == rhs.mIndex && lhs.mInstance == rhs.mInstance); }
    friend bool operator!=(MemHandle lhs, MemHandle rhs) { return !(lhs == rhs); }
    friend bool operator<(MemHandle lhs, MemHandle rhs)  { return (lhs.mIndex < rhs.mIndex && lhs.mInstance < rhs.mInstance); }
    friend bool operator>(MemHandle lhs, MemHandle rhs)  { return (lhs.mIndex > rhs.mIndex && lhs.mInstance > rhs.mInstance); }

public:
    MemHandle()
        : mInstance(U16_MAX)
        , mIndex(U16_MAX)
    {
    }

    MemHandle(u16 instance, u16 index)
        : mInstance(instance)
        , mIndex(index)
    {
    }

    u16 GetInstance() const     { return mInstance; }
    u16 GetIndex() const        { return mIndex; }

    bool IsValid() const        { return (mIndex != U16_MAX && mInstance != U16_MAX); }
    void Invalidate()           { mIndex = U16_MAX; mInstance = U16_MAX; }

private:
    u16 mInstance;   // How many times this slot has been re-used
    u16 mIndex;      // Slot in the pool
};

#endif // #ifndef INCLUDED_ENGINE_MEMHANDLE_H