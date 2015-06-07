#include "Precompiled.h"
#include "MetaRegistration.h"

#include "GameObject.h"
#include "Components.h"
#include "Services.h"

void Meta::MetaRegistration()
{
    #define META_REGISTER(TYPE)\
        MetaDB::Register(TYPE::StaticGetMetaClass())

    // Components
    META_REGISTER(ColliderComponent);
    META_REGISTER(CameraComponent);
    META_REGISTER(MeshComponent);
    META_REGISTER(MeshRendererComponent);
    META_REGISTER(RigidBodyComponent);
    META_REGISTER(TransformComponent);
    META_REGISTER(TerrainComponent);

    // Services
    META_REGISTER(PhysicsService);
    META_REGISTER(RenderService);
    META_REGISTER(TerrainService);

    // Other types
    META_REGISTER(Terrain::Layer);
}