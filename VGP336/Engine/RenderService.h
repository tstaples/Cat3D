#ifndef INCLUDED_ENGINE_RENDERSERVICE_H
#define INCLUDED_ENGINE_RENDERSERVICE_H

//====================================================================================================
// Filename:	RenderService.h
// Created by:	Tyler Staples
// Description: Class representing a Render service.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Service.h"
#include "MeshRenderer.h"
#include "RepositoryTypes.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

class GraphicsSystem;

//====================================================================================================
// Struct
//====================================================================================================

// Data needed to render
struct RenderInfo
{
    // Component ID's
    ID transformId;
    ID modelId;
};

//====================================================================================================
// Class Declarations
//====================================================================================================

class RenderService : public Service<RenderInfo>
{
public:
    RenderService();
    ~RenderService();
    
    void Initialize(GraphicsSystem& graphicsSystem, 
                    GameObjectRepository& gameObjectRepo, 
                    TransformRepository& transformRepo, 
                    ModelRepository& modelRepo,
                    Camera& camera);
    void Terminate();

    void Update();

private:
    virtual void OnSubscribe(ID ObjId, RenderInfo& info);

private:
    GraphicsSystem* mpGraphicsSystem;
    MeshRenderer mRenderer;

    GameObjectRepository* mpGameObjectRepo;
    TransformRepository* mpTransformRepo;
    ModelRepository* mpModelRepo;
};

#endif // #ifndef INCLUDED_ENGINE_RENDERSERVICE_H