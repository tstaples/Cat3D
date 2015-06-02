#ifndef INCLUDED_ENGINE_RIGIDBODYCOMPONENT_H
#define INCLUDED_ENGINE_RIGIDBODYCOMPONENT_H

//====================================================================================================
// Filename:	RigidBodyComponent.h
// Created by:	Tyler Staples
// Description: Class representing a RigidBody Component.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Component.h"
#include "EngineMath.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

class RigidBodyComponent : public Component
{
public:
    META_DECLARE_CLASS

    RigidBodyComponent();
    ~RigidBodyComponent();

    virtual void Update(f32 deltaTime) override;

private:
    f32 mMass;
    f32 mDrag;
    f32 mAngularDrag;
    bool mUseGravity;
    bool mIsKinematic;
};

//====================================================================================================
// Typedefs
//====================================================================================================

//typedef MemHandle<ColliderComponent> ColliderHandle;

#endif // #ifndef INCLUDED_ENGINE_RIGIDBODYCOMPONENT_H