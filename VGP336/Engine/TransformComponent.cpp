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

void TransformComponent::Load(Json::Value& properties)
{
    Json::Value pos = properties.get("Position", "");

}