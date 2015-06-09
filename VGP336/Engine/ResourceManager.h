#ifndef INCLUDED_ENGINE_RESOURCEMANAGER_H
#define INCLUDED_ENGINE_RESOURCEMANAGER_H

//====================================================================================================
// Filename:	ResourceManager.h
// Created by:	Tyler Staples
// Description:	Templatized base class for managing resources.
//              Assumes creation/destruction before resources allocated/released.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Hash.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

template<typename T>
class ResourceManager
{
    typedef std::map<u64, T*> Inventory;

public:
    ResourceManager();
    virtual ~ResourceManager();

    // Checks refcounts to ensure no refs exist for that item
    // then deletes it.
    void Terminate();

    // Hash filename and check if it already exists.
    // If it does, returns a pointer to it
    // else allocates it then returns the pointer
    T* GetResource(const wchar_t* pfilename);
    T* GetResource(const char* pfilename);

private:
    NONCOPYABLE(ResourceManager);

    virtual void InitResource(T* res, const wchar_t* pFilename) = 0;
    virtual void CleanupResource(T* res) = 0;

private:
    Inventory mInventory;
};

//====================================================================================================
// Inline definitions
//====================================================================================================

#include "ResourceManager.inl"

#endif // #ifndef INCLUDED_ENGINE_RESOURCEMANAGER_H