#include "Precompiled.h"
#include "TransformComponent.h"

META_CLASS_BEGIN(TransformComponent)
META_FIELD_BEGIN
    META_FIELD(mPosition, "position")
    META_FIELD(mRotation, "rotation")
    META_FIELD(mScale, "scale")
META_FIELD_END
META_CLASS_END

TransformComponent::TransformComponent()
    : mScale(1.0f, 1.0f, 1.0f)
{
}

//----------------------------------------------------------------------------------------------------

TransformComponent::TransformComponent(const Math::Matrix& transform)
    : mTransform(transform)
    , mScale(1.0f, 1.0f, 1.0f)
{
}

//----------------------------------------------------------------------------------------------------

TransformComponent::~TransformComponent()
{
}

//----------------------------------------------------------------------------------------------------

Math::Vector3 TransformComponent::GetPosition() const
{
    return mPosition;
}

//----------------------------------------------------------------------------------------------------

Math::Matrix TransformComponent::GetTransform() const
{
    Math::Matrix rot =  Math::Matrix::RotationX(mRotation.x) * 
                        Math::Matrix::RotationY(mRotation.y) * 
                        Math::Matrix::RotationZ(mRotation.z);
    return Math::Matrix::Scaling(mScale) * rot * Math::Matrix::Translation(mPosition);
}

//----------------------------------------------------------------------------------------------------

void TransformComponent::Translate(const Math::Vector3& t)
{
    mPosition += t;
    mTransform = mTransform * Math::Matrix::Translation(t);
}