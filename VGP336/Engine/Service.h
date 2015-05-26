#ifndef INCLUDED_ENGINE_SERVICE_H
#define INCLUDED_ENGINE_SERVICE_H

//====================================================================================================
// Filename:	Service.h
// Created by:	Tyler Staples
// Description: Base class for a game logic service. The service maintains a collection
//              of subscribers for processing.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "GameObject.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

class Service
{
public:
    Service(const char* name);
    virtual ~Service();

    const char* GetName() const { return mName; }

    // Subscribes the gameobject to this service
    void Subscribe(GameObjectHandle handle);
    // UnSubscribes the gameobject to this service
    void UnSubscribe(GameObjectHandle handle);

    // Clears subscriber list
    void UnSubscribeAll();

private:
    NONCOPYABLE(Service);

    // Populates the info for the particular ObjId
    virtual void OnSubscribe(GameObjectHandle handle) = 0;

    const char* mName;

protected:
    typedef std::vector<GameObjectHandle> Subscribers;
    Subscribers mSubscribers;
};

#endif // #ifndef INCLUDED_ENGINE_SERVICE_H