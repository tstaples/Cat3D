//====================================================================================================
// Filename:	Service.inl
// Created by:	Tyler Staples
//====================================================================================================

#include "Precompiled.h"
#include "Service.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

Service::Service(const char* name)
    : mName(name)
{
}

// ---------------------------------------------------------------------------------------------------

Service::~Service()
{
}

// ---------------------------------------------------------------------------------------------------

void Service::Subscribe(GameObjectHandle handle)
{
    // Invoke derived implementation
    OnSubscribe(handle);
    mSubscribers.push_back(handle);
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
            mSubscribers.erase(it);
            found = true;
            break;
        }
    }
    ASSERT(found, "[Service] Cannot unsubscribe from service the object is not subscribed to");
}