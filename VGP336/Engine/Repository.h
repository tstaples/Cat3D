#ifndef INCLUDED_ENGINE_REPOSITORY_H
#define INCLUDED_ENGINE_REPOSITORY_H

//====================================================================================================
// Filename:	Repository.h
// Created by:	Tyler Staples
// Description: Templatized class for a type repository.
//              Can only store metatype aware types.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "ID.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

class RepositoryBase
{
public:
    RepositoryBase(Meta::Type type)
        : mType(type)
    {
    }

    Meta::Type GetType() const { return mType; }

private:
    Meta::Type mType;
};

//----------------------------------------------------------------------------------------------------

template<typename T>
class Repository : public RepositoryBase
{
public:
    // ID is a u16
    // @param capacity: capacity of mData
    Repository(Meta::Type type, u16 capacity);

    ID Allocate();
    void Free(ID& id);

    // Assumes the item does exist. Else assert
    T& GetItem(ID id);
    const T& GetItem(ID id) const;

    // Returns nullptr if the item doesn't exist
    T* FindItem(ID id);
    const T* FindItem(ID id) const;

private:
    bool IsValid(ID id) const;

    std::vector<T> mData;           // The actual data
    std::vector<u8> mInstanceCount; // How many times a slot has been used
    std::vector<u16> mFreeSlots;    // If empty, new items must be added to end
};

//====================================================================================================
// Inline Declarations
//====================================================================================================

#include "Repository.inl"

#endif // #ifndef INCLUDED_ENGINE_REPOSITORY_H