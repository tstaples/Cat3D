//====================================================================================================
// Filename:	Service.inl
// Created by:	Tyler Staples
//====================================================================================================

#include "Precompiled.h"
#include "Service.h"

#include "GameWorld.h"
#include "MemoryPool.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

GameWorld* Service::spGameWorld = nullptr;

Service::Service(const char* name, u16 id)
    : mName(name)
    , mID(id)
{
}

// ---------------------------------------------------------------------------------------------------

Service::~Service()
{
}

// ---------------------------------------------------------------------------------------------------

bool Service::Subscribe(GameObjectHandle handle)
{
    // Check the GameObject isn't already subscribed to this service
    GameObject* gameObject = handle.Get();
    if (!gameObject->HasService(mID))
    {
        // Invoke derived implementation
        if (OnSubscribe(handle))
        {
            gameObject->AddService(mID);
            mSubscribers.push_back(handle);
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------------------------------------------------

void Service::UnSubscribe(GameObjectHandle handle)
{
    bool found = false;
    Subscribers::iterator it = mSubscribers.begin();
    for (it; it != mSubscribers.end(); ++it)
    {
        if (*it == handle)
        {
            GameObject* gameObject = handle.Get();
            gameObject->RemoveService(mID);
            mSubscribers.erase(it);
            found = true;
            break;
        }
    }
    ASSERT(found, "[Service] Cannot unsubscribe from service the object is not subscribed to");
}

// ---------------------------------------------------------------------------------------------------

void Service::UnSubscribeAll()
{
    mSubscribers.clear();
}

// ---------------------------------------------------------------------------------------------------

void Service::Terminate()
{
    UnSubscribeAll();
}