#include "Precompiled.h"
#include "RigidBodyComponent.h"

#include "GameObject.h"
#include "GameWorld.h"
#include "TransformComponent.h"

META_CLASS_BEGIN(RigidBodyComponent)
META_FIELD_BEGIN
    META_FIELD(mMass, "Mass")
    META_FIELD(mDrag, "Drag")
    META_FIELD(mAngularDrag, "AngularDrag")
    META_FIELD(mUseGravity, "UseGravity")
    META_FIELD(mIsKinematic, "IsKinematic")
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
    const GameWorld* world = GetObj()->GetWorld();
    const f32 kGravity = world->GetGameSettings().gravity;

    LOG("deltaTime: %f", deltaTime);
    TransformComponent* transformComponent = nullptr;
    GetObj()->GetComponent(transformComponent);

    Math::Vector3 pos = transformComponent->GetPosition();
    if (mUseGravity)
    {
        mVelocity.y += kGravity * deltaTime;
    }
    pos += mVelocity * deltaTime;
    transformComponent->SetPosition(pos);
}