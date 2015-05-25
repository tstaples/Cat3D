//====================================================================================================
// Filename:	GameWorld.h
// Created by:	Tyler Staples
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"
#include "GameWorld.h"

//====================================================================================================
// local Definitions
//====================================================================================================

GameWorld::GameWorld(u16 maxObjects)
    : mGameObjectPool(maxObjects)
    , mFactory(mGameObjectPool)
    , mOctree(Math::AABB(Math::Vector3::Zero(), Math::Vector3(100.0f, 100.0f, 100.0f))) // TODO: size dynamically or something
{
}

//----------------------------------------------------------------------------------------------------

GameWorld::~GameWorld()
{
}

//----------------------------------------------------------------------------------------------------

bool GameWorld::OnInitialize(const GameSettings& settings, GraphicsSystem& gs, Camera& camera)
{
    mSettings = settings;

    // Init services
    mRenderService.Initialize(gs, camera);

    // Store all our services in a list to pass to the GameObjectFactory
    Services services =
    {
        &mRenderService
    };
    mFactory.Initialize(services);
    return true;
}

//----------------------------------------------------------------------------------------------------

bool GameWorld::OnShutdown()
{
    mFactory.Terminate();
    mOctree.Destroy();
    mGameObjectHandles.clear();
    mGameObjectPool.Flush();
    return true;
}

//----------------------------------------------------------------------------------------------------

bool GameWorld::OnInput()
{
    // TODO: Invoke script "OnInput" function
    return true;
}

//----------------------------------------------------------------------------------------------------

void GameWorld::OnUpdate(f32 deltaTime)
{
    // Reconstruct the Octree
    mOctree.Destroy();
    for (GameObjectHandle& handle : mGameObjectHandles)
    {
        GameObject* gameObject = handle.Get();

        // TODO: If we decide not to have every gameobject have a transform, check for one here too.

        ColliderComponent* colliderComponent = nullptr;
        if (gameObject->FindComponent(colliderComponent))
        {
            // Only add objects to the tree that contain colliders
            Math::AABB region = colliderComponent->GetBoundary();
            mOctree.Insert(handle, region); 
        }
    }

    // Update all the game objects
    for (GameObjectHandle& handle : mGameObjectHandles)
    {
        GameObject* gameObject = handle.Get();
        gameObject->Update(deltaTime);
    }

    // TODO: Update other services
}

//----------------------------------------------------------------------------------------------------

void GameWorld::OnRender()
{
    mRenderService.Update();
}

//----------------------------------------------------------------------------------------------------

void GameWorld::CreateGameObject(const char* templateFile, Math::Vector3 pos, Math::Quaternion rot)
{
    // Use the templateFile if it's not null, otherwise use the default object template.
    // Note: For now we're assuming all GameObjects have a transform component
    const char* templatePath = (templateFile) ? templateFile : "../Data/GameObjects/default.json";
    GameObjectHandle handle = mFactory.Create(templatePath);

    GameObject* gameObject = handle.Get();
    TransformComponent* transformComponent = nullptr;
    if (gameObject->GetComponent(transformComponent))
    {
        transformComponent->Translate(pos);
        // TODO: transformComponent->Rotate(rot);
    }

    // Add the object to the world
    mGameObjectHandles.push_back(handle);
}