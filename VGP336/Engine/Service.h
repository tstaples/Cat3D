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
    Service(const char* name, u16 id);
    virtual ~Service();

    const char* GetName() const { return mName; }

    // Subscribes the gameobject to this service
    bool Subscribe(GameObjectHandle handle);
    
    // UnSubscribes the gameobject to this service
    void UnSubscribe(GameObjectHandle handle);

    // Clears subscriber list
    void UnSubscribeAll();

    virtual void Terminate() = 0;

    u16 GetID() const { return mID; }

private:
    NONCOPYABLE(Service);

    // Populates the info for the particular ObjId
    virtual bool OnSubscribe(GameObjectHandle handle) = 0;

    const char* mName;

protected:
    typedef std::vector<GameObjectHandle> Subscribers;
    Subscribers mSubscribers;
    u16 mID;
};

#endif // #ifndef INCLUDED_ENGINE_SERVICE_H