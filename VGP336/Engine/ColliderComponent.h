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
    ColliderComponent();
    ~ColliderComponent();

    virtual void Load(Json::Value& properties);

    Math::AABB& GetBoundary()                { return mAABB; }
    const Math::AABB& GetBoundary() const    { return mAABB; }

private:
    Math::AABB mAABB;
};

#endif // #ifndef INCLUDED_ENGINE_COLLIDERCOMPONENT_H