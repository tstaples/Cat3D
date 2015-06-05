//====================================================================================================
// Filename:	PhysicsService.h
// Created by:	Tyler Staples
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"
#include "PhysicsService.h"

#include "EngineMath.h"
#include "GameWorld.h"
#include "MemHandle.h"
#include "MemoryPool.h"
#include "ColliderComponent.h"
#include "RigidBodyComponent.h"
#include "TransformComponent.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

META_CLASS_BEGIN(PhysicsService)
META_DEPENDENCIES_BEGIN
    META_DEPENDENCY("TransformComponent", "Component")
META_DEPENDENCIES_END
META_CLASS_END

PhysicsService::PhysicsService()
    : Service("PhysicsService", kID)
    , mOctree(Math::AABB(0.0f, 0.0f, 0.0f, 
              Math::Vector3(1.0f, 1.0f, 1.0f)),
              1, 10)
{
}

//----------------------------------------------------------------------------------------------------

PhysicsService::~PhysicsService()
{
}

//----------------------------------------------------------------------------------------------------

void PhysicsService::Initialize(const Math::AABB& worldRegion, s32 maxDepth)
{
    mOctree.SetBoundingRegion(worldRegion);
    mOctree.SetMaxDepth(maxDepth);
}

//----------------------------------------------------------------------------------------------------

void PhysicsService::Terminate()
{
    Service::Terminate();
}

//----------------------------------------------------------------------------------------------------

void PhysicsService::Update(f32 deltaTime)
{
    if (mSubscribers.empty())
    {
        return;
    }

    Integrate(deltaTime);

    // Recreate the tree every frame
    // TODO: Use pool for oct allocation
    mOctree.Destroy();

    // Iterate through all the subscribers and render them
    Subscribers::iterator it = mSubscribers.begin();
    for (it; it != mSubscribers.end(); ++it)
    {
        GameObject* gameObject = it->Get();
        TransformComponent* transformComponent = nullptr;
        RigidBodyComponent* rigidBodyComponent = nullptr;
        ColliderComponent* colliderComponent = nullptr;

        gameObject->GetComponent(transformComponent);
        bool hasRigidBody = gameObject->FindComponent(rigidBodyComponent);
        bool hasCollider = gameObject->FindComponent(colliderComponent);
        
        Math::Vector3 pos = transformComponent->GetPosition();
        Math::Vector3 oldPos = transformComponent->GetOldPosition();

        // Get the collider if the object has one
        Math::AABB collider(pos, Math::Vector3(1.0f, 1.0f, 1.0f)); // default
        if (hasCollider)
        {
            // Use the collider's extend if the object has one
            Math::AABB aabb = colliderComponent->GetBoundary();
            collider.center = pos + aabb.center; // Offset the collider from object position
            collider.extend = aabb.extend;
        }
        
        // Insert the item into the tree
        mOctree.Insert(*it, collider);

        // TODO: do collision resolution here as the object's positions will have been updated
        // by their components.
    }
    mOctree.Debug_DrawTree();
}

//----------------------------------------------------------------------------------------------------

void PhysicsService::Integrate(f32 deltaTime)
{
    const GameWorld* world = GetWorld();
    const f32 kTimeStep = Math::Sqr(world->GetGameSettings().timeStep);
    for (GameObjectHandle handle : mSubscribers)
    {
        // Get component data
        GameObject* gameObject = handle.Get();
        TransformComponent* transformComponent = nullptr;
        RigidBodyComponent* rigidBodyComponent = nullptr;
        gameObject->GetComponent(transformComponent);
        if (!gameObject->FindComponent(rigidBodyComponent))
        {
            continue;
        }

        Math::Vector3 pos = transformComponent->GetPosition();
        Math::Vector3 vel = rigidBodyComponent->GetVelocity();

        f32 kInvdrag = 1.0f - rigidBodyComponent->GetDrag();
        Math::Vector3 gravity(0.0f, 0.0f, 0.0f);
        if (rigidBodyComponent->UseGravity())
        {
            gravity = world->GetGameSettings().gravity;
            vel += gravity * deltaTime;
        }
        rigidBodyComponent->SetVelocity(vel);
        transformComponent->SetPosition(pos + vel * deltaTime);
    }
}

//----------------------------------------------------------------------------------------------------

bool PhysicsService::OnSubscribe(GameObjectHandle handle)
{
    GameObject* gameObject = handle.Get();
    TransformComponent* transformComponent = nullptr;
    
    // Ensure the GameObject has the required components
    if (!gameObject->FindComponent(transformComponent))
    {
        return false;
    }
    return true;
}