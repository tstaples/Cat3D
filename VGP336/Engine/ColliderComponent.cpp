#include "Precompiled.h"
#include "ColliderComponent.h"

#include <json/json.h>

ColliderComponent::ColliderComponent()
{
}

//----------------------------------------------------------------------------------------------------

ColliderComponent::~ColliderComponent()
{
}

//----------------------------------------------------------------------------------------------------

void ColliderComponent::Load(Json::Value& properties)
{
    Json::Value pos = properties.get("Position", "");

}