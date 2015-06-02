#include "Precompiled.h"
#include "RigidBodyComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"

META_CLASS_BEGIN(RigidBodyComponent)
META_FIELD_BEGIN
    META_FIELD(mMass, "Mass")
    META_FIELD(mDrag, "Drag")
    META_FIELD(mAngularDrag, "AngularDrag")
    META_FIELD(mUseGravity, "Use Gravity")
    META_FIELD(mIsKinematic, "Is Kinematic")
META_FIELD_END
META_DEPENDENCIES_BEGIN
    META_DEPENDENCY("PhysicsService", "Service")
META_DEPENDENCIES_END
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

//----------------------------------------------------------------------------------------------------

void RigidBodyComponent::Update(f32 deltaTime)
{
    const f32 kGravity = -9.8f; // temp until read from settings

    LOG("deltaTime: %f", deltaTime);
    TransformComponent* transformComponent = nullptr;
    GetObj()->GetComponent(transformComponent);

    Math::Vector3 pos = transformComponent->GetPosition();
    mVelocity.y += kGravity * deltaTime;
    pos += mVelocity * deltaTime;
    transformComponent->SetPosition(pos);
}