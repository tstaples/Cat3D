#include "Precompiled.h"

#include "MetaRegistration.h"

#include "GameObject.h"
#include "Component.h"

// Components (TODO: put in single "Components" header
#include "TransformComponent.h"
#include "ColliderComponent.h"

void Meta::MetaRegistration()
{
    MetaDB::Register(TransformComponent::StaticGetMetaClass());
    MetaDB::Register(ColliderComponent::StaticGetMetaClass());
}