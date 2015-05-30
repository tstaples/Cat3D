#include "Precompiled.h"
#include "MetaRegistration.h"

#include "GameObject.h"
#include "Components.h"
#include "Services.h"

void Meta::MetaRegistration()
{
    MetaDB::Register(ColliderComponent::StaticGetMetaClass());
    MetaDB::Register(CameraComponent::StaticGetMetaClass());
    MetaDB::Register(MeshComponent::StaticGetMetaClass());
    MetaDB::Register(MeshRendererComponent::StaticGetMetaClass());
    MetaDB::Register(RigidBodyComponent::StaticGetMetaClass());
    MetaDB::Register(TransformComponent::StaticGetMetaClass());

    MetaDB::Register(PhysicsService::StaticGetMetaClass());
    MetaDB::Register(RenderService::StaticGetMetaClass());
}