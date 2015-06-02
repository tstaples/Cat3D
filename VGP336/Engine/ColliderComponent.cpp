#include "Precompiled.h"
#include "ColliderComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"

META_CLASS_BEGIN(ColliderComponent)
META_FIELD_BEGIN
    META_FIELD(mCenter, "Center")
    META_FIELD(mExtend, "Extend")
    META_FIELD(mIsTrigger, "IsTrigger")
META_FIELD_END
META_DEPENDENCIES_BEGIN
    META_DEPENDENCY("PhysicsService", "Service")
META_DEPENDENCIES_END
META_CLASS_END

ColliderComponent::ColliderComponent()
    : mIsTrigger(false)
{
}

//----------------------------------------------------------------------------------------------------

ColliderComponent::~ColliderComponent()
{
}

//----------------------------------------------------------------------------------------------------

void ColliderComponent::Update(f32 deltaTime)
{
}

//----------------------------------------------------------------------------------------------------

void ColliderComponent::SetBoundary(const Math::AABB& aabb)
{
    mCenter = aabb.center;
    mExtend = aabb.extend;
}

//----------------------------------------------------------------------------------------------------

Math::AABB ColliderComponent::GetBoundary() const
{
    return Math::AABB(mCenter, mExtend);
}