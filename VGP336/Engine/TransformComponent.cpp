#include "Precompiled.h"
#include "TransformComponent.h"

META_CLASS_BEGIN(TransformComponent)
META_FIELD_BEGIN
    META_FIELD(mTransform, "transform")
META_FIELD_END
META_CLASS_END

TransformComponent::TransformComponent(GameObject* gameObject)
    : Component(gameObject)
{
}

//----------------------------------------------------------------------------------------------------

TransformComponent::TransformComponent(GameObject* gameObject, const Math::Matrix& transform)
    : Component(gameObject)
    , mTransform(transform)
{
}

//----------------------------------------------------------------------------------------------------

TransformComponent::~TransformComponent()
{
}

//----------------------------------------------------------------------------------------------------

Math::Vector3 TransformComponent::GetPosition() const
{
    return mTransform.GetTranslation();
}

//----------------------------------------------------------------------------------------------------

void TransformComponent::Translate(const Math::Vector3& t)
{
    mTransform = mTransform * Math::Matrix::Translation(t);
}