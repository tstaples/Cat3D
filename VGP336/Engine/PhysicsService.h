#ifndef INCLUDED_ENGINE_PHYSICSSERVICE_H
#define INCLUDED_ENGINE_PHYSICSSERVICE_H

//====================================================================================================
// Filename:	PhysicsService.h
// Created by:	Tyler Staples
// Description: Class representing a Physics service.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Service.h"

#include "GameObject.h"
#include "Octree.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================


//====================================================================================================
// Class Declarations
//====================================================================================================

class PhysicsService : public Service
{
public:
    META_DECLARE_CLASS

    static const u16 kID = 1;

    PhysicsService();
    ~PhysicsService();
    
    void Initialize(const Math::AABB& worldRegion, s32 maxDepth);
    void Update(f32 deltaTime);
    virtual void Terminate();

private:
    virtual bool OnSubscribe(GameObjectHandle handle);

    Octree<GameObjectHandle> mOctree;
};

#endif // #ifndef INCLUDED_ENGINE_RENDERSERVICE_H