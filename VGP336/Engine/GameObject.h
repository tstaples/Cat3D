#ifndef INCLUDED_ENGINE_GAMEOBJECT_H
#define INCLUDED_ENGINE_GAMEOBJECT_H

//====================================================================================================
// Filename:	GameObject.h
// Created by:	Tyler Staples
// Description: Class representing a GameObject.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "MemHandle.h"

// Temp
#include "TransformComponent.h"
#include "MetaTypes.h"
#include "ID.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

class Component;

//====================================================================================================
// Class Declarations
//====================================================================================================

class GameObject
{
public:
    typedef std::vector<Component*> Components;
    typedef std::vector<Meta::Type> ServiceList;

public:
    GameObject();
    ~GameObject();

    void AddComponent(ID id);
    void AddService(Meta::Type type);

    //ID GetComponentID(Meta::Type type);   // Garuntees ID
    //ID FindComponentID(Meta::Type type);  // Returns invalid ID if component doesn't exist

    // TEMP UNTIL META SYSTEM DONE
    TransformComponent& GetTransform() { return mTransform; }

    // TODO: meta system for getting type
    // Returns bool rather than pointer to force caching local pointer
    template<typename T>
    bool GetComponent(T*& component);
    template<typename T>
    bool GetComponent(T*& component) const;

    template<typename T>
    bool FindComponent(T*& component);
    template<typename T>
    bool FindComponent(T*& component) const;

    const char* GetName() const                 { return mName.c_str(); }
    ID GetID() const                            { return mID; }
    const Components& GetComponents() const   { return mComponents; }
    const ServiceList& GetServices() const      { return mServices; }

private:
    //NONCOPYABLE(GameObject)

    std::string mName; // See TODO in TString
    ID mID;

    // TEMP UNTIL META SYSTEM DONE
    TransformComponent mTransform;

    Components mComponents;
    ServiceList mServices;
};

#endif // #ifndef INCLUDED_ENGINE_GAMEOBJECT_H