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

//#include "ID.h"
//#include "Component.h"
//
////====================================================================================================
//// Class Declarations
////====================================================================================================
//
//class RepositoryBase
//{
//public:
//    RepositoryBase(Meta::Type type)
//        : mType(type)
//    {
//    }
//    virtual ~RepositoryBase() {}
//
//    Meta::Type GetType() const { return mType; }
//
//    virtual ID Allocate() = 0;
//
//    //virtual Component* GetComponent(ID id) = 0;
//
//private:
//    NONCOPYABLE(RepositoryBase);
//
//    Meta::Type mType;
//};
//
////----------------------------------------------------------------------------------------------------
//
//template<typename T>
//class Repository : public RepositoryBase
//{
//public:
//    // ID is a u16
//    // @param capacity: capacity of mData
//    Repository(Meta::Type type, u16 capacity);
//    ~Repository();
//
//    virtual ID Allocate();
//    void Free(ID& id);
//
//    void Flush();
//
//    // Assumes the item does exist. Else assert
//    T& GetItem(ID id);
//    const T& GetItem(ID id) const;
//
//    // Returns nullptr if the item doesn't exist
//    T* FindItem(ID id);
//    const T* FindItem(ID id) const;
//
//    //virtual Component* GetComponent(ID id);
//
//private:
//    bool IsValid(ID id) const;
//
//    T* mData;                       // The actual data
//    u8* mInstanceCount;             // How many times a slot has been used
//
//    u16 mSize;
//    u16 mCapacity;
//
//    std::vector<u16> mFreeSlots;    // If empty, new items must be added to end
//};
//
////====================================================================================================
//// Inline Declarations
////====================================================================================================
//
//#include "Repository.inl"

#endif // #ifndef INCLUDED_ENGINE_REPOSITORY_H