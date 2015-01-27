#ifndef INCLUDED_ENGINE_SERVICE_H
#define INCLUDED_ENGINE_SERVICE_H

//====================================================================================================
// Filename:	Service.h
// Created by:	Tyler Staples
// Description: Templatized Base class for a game logic service. The service maintains a collection
//              of subscribers for processing.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "ID.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

template<typename T>
class Service
{
public:
    Service();
    virtual ~Service();

    // Subscribes the gameobject to this service
    void Subscribe(ID ObjId);
    // UnSubscribes the gameobject to this service
    void UnSubscribe(ID ObjId);

private:
    // Populates the info for the particular ObjId
    virtual void OnSubscribe(ID ObjId, T& info) = 0;

protected:
    // GameObject ID, 
    typedef std::map<ID, T> Subscribers;

    NONCOPYABLE(Service);

protected:
    Subscribers mSubscribers;
};

//====================================================================================================
// Inline Declarations
//====================================================================================================

#include "Service.inl"

#endif // #ifndef INCLUDED_ENGINE_SERVICE_H