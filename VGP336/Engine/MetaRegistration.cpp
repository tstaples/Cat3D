#include "Precompiled.h"
#include "MetaRegistration.h"

#include "GameObject.h"
#include "Components.h"

void Meta::MetaRegistration()
{
    MetaDB::Register(ColliderComponent::StaticGetMetaClass());
    MetaDB::Register(MeshComponent::StaticGetMetaClass());
    MetaDB::Register(MeshRendererComponent::StaticGetMetaClass());
    MetaDB::Register(TransformComponent::StaticGetMetaClass());
}