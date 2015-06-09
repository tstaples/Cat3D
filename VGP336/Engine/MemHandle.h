#ifndef INCLUDED_ENGINE_MEMHANDLE_H
#define INCLUDED_ENGINE_MEMHANDLE_H

//====================================================================================================
// Filename:	MemHandle.h
// Created by:	Tyler Staples
// Description: Class for a handle to a memory block.
//====================================================================================================

template <typename DataType> class MemoryPool;

//====================================================================================================
// Class Declarations
//====================================================================================================

template <typename DataType>
class MemHandle
{
    friend bool operator==(MemHandle lhs, MemHandle rhs) { return (lhs.mIndex == rhs.mIndex && lhs.mInstance == rhs.mInstance); }
    friend bool operator!=(MemHandle lhs, MemHandle rhs) { return !(lhs == rhs); }
    friend bool operator<(MemHandle lhs, MemHandle rhs)  { return (lhs.mIndex < rhs.mIndex && lhs.mInstance < rhs.mInstance); }
    friend bool operator>(MemHandle lhs, MemHandle rhs)  { return (lhs.mIndex > rhs.mIndex && lhs.mInstance > rhs.mInstance); }

public:
    // Default constructor initializes to invalid
    MemHandle();
    MemHandle(u16 instance, u16 index);

    DataType* Get();
    const DataType* Get() const;

    u16 GetInstance() const     { return mInstance; }
    u16 GetIndex() const        { return mIndex; }

    bool IsValid() const;
    void Invalidate();

private:
    friend class MemoryPool<DataType>;
    static MemoryPool<DataType>* sMemoryPool;

    u16 mInstance;   // How many times this slot has been re-used
    u16 mIndex;      // Slot in the pool
};

//====================================================================================================
// Inline Definitions
//====================================================================================================

#include "MemHandle.inl"

#endif // #ifndef INCLUDED_ENGINE_MEMHANDLE_H