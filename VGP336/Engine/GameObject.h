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

#include "ID.h"

class Component;

//====================================================================================================
// Class Declarations
//====================================================================================================


class GameObject
{
public:
    typedef std::map<Meta::Type, ID> ComponentMap;
    typedef std::vector<Meta::Type> ServiceList;

public:
    GameObject();
    ~GameObject();

    void AddComponent(ID id);
    void AddService(Meta::Type type);

    ID GetComponentID(Meta::Type type);   // Garuntees ID
    ID FindComponentID(Meta::Type type);  // Returns invalid ID if component doesn't exist

    Component* GetComponent(Meta::Type type);

    const char* GetName() const                 { return mName.c_str(); }
    ID GetID() const                            { return mID; }
    const ComponentMap& GetComponents() const   { return mComponents; }
    const ServiceList& GetServices() const      { return mServices; }

    // Hack: expose members since there isn't a factory to assign them values yet

//private:
    //NONCOPYABLE(GameObject)

    std::string mName; // See TODO in TString
    ID mID;

    ComponentMap mComponents;
    ServiceList mServices;
};

#endif // #ifndef INCLUDED_ENGINE_GAMEOBJECT_H