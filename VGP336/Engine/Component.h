#ifndef INCLUDED_ENGINE_COMPONENT_H
#define INCLUDED_ENGINE_COMPONENT_H

//====================================================================================================
// Filename:	Component.h
// Created by:	Tyler Staples
// Description: Base Class for a component.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Meta.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

class GameObject;

//====================================================================================================
// Class Declarations
//====================================================================================================

class Component
{
public:
    META_DECLARE_CLASS

    Component(GameObject* gameObject);
    virtual ~Component();

    virtual void Initialize() {}
    virtual void Terminate() {}

    GameObject* GetObj()                { return mpGameObject; }
    const GameObject* GetObj() const    { return mpGameObject; }

private:
    GameObject* mpGameObject;
};

#endif // #ifndef INCLUDED_ENGINE_COMPONENT_H