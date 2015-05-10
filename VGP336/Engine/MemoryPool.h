#ifndef INCLUDED_ENGINE_MEMORYPOOL_H
#define INCLUDED_ENGINE_MEMORYPOOL_H

//====================================================================================================
// Filename:	MemoryPool.h
// Created by:	Tyler Staples
// Description: Templatized class for a type repository.
//              Can only store metatype aware types.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "MemHandle.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

// Handle type
template<typename DataType>
class MemoryPool
{
public:
    // Constructor.
    // @param capacity: capacity of mData.
    // Note: MemHandle is a u16.
    MemoryPool(u16 capacity);
    ~MemoryPool();

    // Reserves a slot.
    // Returns: handle associated with the allocated block.
    MemHandle Allocate();

    // Free's the slot referred to by the handle.
    // @param handle: handle associated with the slot to be freed.
    void Free(MemHandle<DataType>& handle);

    // Clear everything (ungraceful).
    void Flush();

    // Validates the memory associated with the handle.
    // @param handle: handle to verify.
    // Returns: true if the handle is valid.
    bool IsValid(MemHandle<DataType> handle) const;

    // Gets a pointer to the memory associated with the handle.
    // @param handle: handle associated with desired memory.
    // Returns: pointer to the data, or null if it doesn't exist.
    DataType* GetItem(MemHandle<DataType> handle);
    const DataType* GetItem(MemHandle<DataType> handle) const;

    // TODO: store pointer to memory pool so we can have a "Get" method
    // which returns a pointer to the data in the pool. Requires memhandle to be templatized

private:
    NONCOPYABLE(MemoryPool);

    DataType* mData;                // The actual data
    u16* mInstanceCount;            // How many times a slot has been used

    u16 mSize;                      // How much is held currently
    u16 mCapacity;                  // How much can be held

    std::vector<u16> mFreeSlots;    // If empty, new items must be added to end
};

//====================================================================================================
// Inline Declarations
//====================================================================================================

#include "MemoryPool.inl"

#endif // #ifndef INCLUDED_ENGINE_MEMORYPOOL_H