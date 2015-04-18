#include "Precompiled.h"
#include "TransformComponent.h"

#include <json/json.h>

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

//----------------------------------------------------------------------------------------------------

Math::Vector3 TransformComponent::GetPosition() const
{
    return mTransform.GetTranslation();
}

//----------------------------------------------------------------------------------------------------

void TransformComponent::Load(Json::Value& properties)
{
    Json::Value pos = properties.get("Position", "");

}