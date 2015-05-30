#include "Precompiled.h"
#include "RigidBodyComponent.h"

META_CLASS_BEGIN(RigidBodyComponent)
META_FIELD_BEGIN
    META_FIELD(mMass, "Mass")
    META_FIELD(mDrag, "Drag")
    META_FIELD(mAngularDrag, "AngularDrag")
    META_FIELD(mUseGravity, "Use Gravity")
    META_FIELD(mIsKinematic, "Is Kinematic")
META_FIELD_END
META_CLASS_END

RigidBodyComponent::RigidBodyComponent()
    : mMass(1.0f)
    , mDrag(0.0f)
    , mAngularDrag(0.0f)
    , mUseGravity(true)
    , mIsKinematic(false)
{
}

//----------------------------------------------------------------------------------------------------

RigidBodyComponent::~RigidBodyComponent()
{
}