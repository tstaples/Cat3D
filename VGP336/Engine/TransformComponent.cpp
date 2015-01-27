#include "Precompiled.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent()
{
}

//----------------------------------------------------------------------------------------------------

TransformComponent::TransformComponent(const Math::Matrix& transform)
    : mTransform(transform)
{
}

//----------------------------------------------------------------------------------------------------

TransformComponent::~TransformComponent()
{
}