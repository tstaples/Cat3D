#ifndef INCLUDED_ENGINE_COLLIDERCOMPONENT_H
#define INCLUDED_ENGINE_COLLIDERCOMPONENT_H

//====================================================================================================
// Filename:	ColliderComponent.h
// Created by:	Tyler Staples
// Description: Class representing a Collider Component.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Component.h"
#include "EngineMath.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

class ColliderComponent : public Component
{
public:
    META_DECLARE_CLASS

    ColliderComponent();
    ~ColliderComponent();

    void SetBoundary(const Math::AABB& aabb);
    Math::AABB GetBoundary() const;

    bool IsTrigger() const  { return mIsTrigger; }    

private:
    Math::Vector3 mCenter;
    Math::Vector3 mExtend;
    bool mIsTrigger;
};

//====================================================================================================
// Typedefs
//====================================================================================================

//typedef MemHandle<ColliderComponent> ColliderHandle;

#endif // #ifndef INCLUDED_ENGINE_COLLIDERCOMPONENT_H