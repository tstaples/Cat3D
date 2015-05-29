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
typedef std::vector<Service*> Services;

class GameWorld;

//====================================================================================================
// Class Declarations
//====================================================================================================

class GameObjectFactory
{
public:
    GameObjectFactory(GameObjectPool& gameObjectPool);
    ~GameObjectFactory();

    void Initialize(Services& services, GameWorld& world);
    void Terminate();

    GameObjectHandle Create(const char* templateFileName);
    GameObjectHandle Create(SerialReader& reader);

    bool AddComponent(GameObjectHandle handle, const char* componentMetaName);

    /*enum ErrorCode
    {
        ServiceDoesNotExist,
        ServiceDependenciesNotMet,
    };*/

private:
    GameObjectPool& mGameObjectPool;
    Services mServices;

    GameWorld* mpWorld;
    // TODO
    // Component memory pools
    //TransformPool mTransformPool;
    //ColliderPool mColliderPool;
    //MeshPool mMeshPool;
};

namespace Exceptions
{
    class MissingDependencyException : public std::exception
    {
    public:
        MissingDependencyException(GameObjectHandle handle, const char* missingDepName)
            : gameObjecthandle(handle)
            , missingDependencyName(missingDepName)
        {
        }
        GameObjectHandle gameObjecthandle;
        const char* missingDependencyName;
    };
}

#endif // #ifndef INCLUDED_ENGINE_GAMEOBJECTFACTORY_H