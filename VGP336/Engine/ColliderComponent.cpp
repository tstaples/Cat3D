#include "Precompiled.h"
#include "ColliderComponent.h"

#include <json/json.h>

ColliderComponent::ColliderComponent(GameObject* gameObject)
    : Component(gameObject)
{
}

//----------------------------------------------------------------------------------------------------

ColliderComponent::~ColliderComponent()
{
}