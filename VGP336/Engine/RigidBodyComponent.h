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

    f32 GetMass() const         { return mMass; }
    f32 GetDrag() const         { return mDrag; }
    f32 GetAngularDrag() const  { return mAngularDrag; }
    bool UseGravity() const     { return mUseGravity; }
    bool IsKinematic() const    { return mIsKinematic; }

    const Math::Vector3& GetVelocity() const { return mVelocity; }
    void SetVelocity(const Math::Vector3& v) { mVelocity = v; }

private:
    f32 mMass;
    f32 mDrag;
    f32 mAngularDrag;
    bool mUseGravity;
    bool mIsKinematic;

    Math::Vector3 mVelocity;
};

//====================================================================================================
// Typedefs
//====================================================================================================

//typedef MemHandle<ColliderComponent> ColliderHandle;

#endif // #ifndef INCLUDED_ENGINE_RIGIDBODYCOMPONENT_H