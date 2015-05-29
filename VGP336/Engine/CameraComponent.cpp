#include "Precompiled.h"
#include "CameraComponent.h"

#include "GameObject.h"
#include "GameWorld.h"

META_CLASS_BEGIN(CameraComponent)
META_FIELD_BEGIN
    META_FIELD(mFov, "Fov")
    META_FIELD(mNear, "Near")
    META_FIELD(mFar, "Far")
META_FIELD_END
META_DEPENDENCIES_BEGIN
    META_DEPENDENCY("TransformComponent")
META_DEPENDENCIES_END
META_CLASS_END

CameraComponent::CameraComponent()
    : mFov(Math::kPiByTwo)
    , mNear(0.01f)
    , mFar(1000.0f)
{
    f32 width = (f32)GetObj()->GetWorld()->GetScreenWidth();
    f32 height = (f32)GetObj()->GetWorld()->GetScreenHeight();
    mCamera.Setup(mFov, width / height, mNear, mFar);
}

//----------------------------------------------------------------------------------------------------

CameraComponent::~CameraComponent()
{
}

//----------------------------------------------------------------------------------------------------

void CameraComponent::Update(f32 deltaTime)
{
    if (IsDirty())
    {
        // TODO: update camera's values (if initialized)
        f32 width = (f32)GetObj()->GetWorld()->GetScreenWidth();
        f32 height = (f32)GetObj()->GetWorld()->GetScreenHeight();
        mCamera.Setup(mFov, width / height, mNear, mFar);
    }
}