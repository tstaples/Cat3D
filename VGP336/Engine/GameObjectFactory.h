#ifndef INCLUDED_ENGINE_GAMEOBJECTFACTORY_H
#define INCLUDED_ENGINE_GAMEOBJECTFACTORY_H

//====================================================================================================
// Filename:	GameObjectFactory.h
// Created by:	Tyler Staples
// Description: Class for a GameObject Factory.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Components.h"
#include "GameObject.h"
#include "MemHandle.h"
#include "MemoryPool.h"
#include "Service.h"

//====================================================================================================
// Typedefs
//====================================================================================================

typedef MemoryPool<TransformComponent> TransformPool;
typedef MemoryPool<ColliderComponent> ColliderPool;
typedef MemoryPool<MeshComponent> MeshPool;
typedef std::map<const char*, Service*> Services;

//====================================================================================================
// Class Declarations
//====================================================================================================

class GameObjectFactory
{
public:
    GameObjectFactory(GameObjectPool& gameObjectPool, u32 componentPoolCapacity);
    ~GameObjectFactory();

    void Initialize(Services& services);
    void Terminate();

    GameObjectHandle Create(const char* templateFileName);

private:
    GameObjectPool& mGameObjectPool;
    Services* mServices;

    // TODO
    // Component memory pools
    //TransformPool mTransformPool;
    //ColliderPool mColliderPool;
    //MeshPool mMeshPool;
};

#endif // #ifndef INCLUDED_ENGINE_GAMEOBJECTFACTORY_H