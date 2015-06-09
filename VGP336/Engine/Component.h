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

#include "MemHandle.h"
#include "Meta.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

class GameObject;
class GameWorld;

//====================================================================================================
// Class Declarations
//====================================================================================================

class Component
{
public:
    META_DECLARE_CLASS

    Component();
    virtual ~Component();

    virtual void Initialize() {}
    virtual void Terminate() {}
    virtual void Update(f32 deltaTime) {} // Not all components need to update; do nothing
    virtual void Render() {}

    void SetIsDirty(bool state)         { mIsDirty = state; }
    bool IsDirty() const                { return mIsDirty; }

    GameObject* GetObj()                { return mpGameObject; }
    const GameObject* GetObj() const    { return mpGameObject; }

    const GameWorld* GetWorld() const;

private:
    friend GameObject;

    GameObject* mpGameObject;

    bool mIsDirty; // Indicates if the component's state has changed
};

#endif // #ifndef INCLUDED_ENGINE_COMPONENT_H