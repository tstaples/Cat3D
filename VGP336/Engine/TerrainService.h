#ifndef INCLUDED_ENGINE_TERRAINSERVICE_H
#define INCLUDED_ENGINE_TERRAINSERVICE_H

//====================================================================================================
// Filename:	TerrainService.h
// Created by:	Tyler Staples
// Description: Class representing a Render service.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Service.h"

#include "AssetLoader.h"
#include "GameObject.h"
#include "MeshRenderer.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

class Camera;
class GraphicsSystem;
class MeshComponent;
class MeshRendererComponent;

//====================================================================================================
// Class Declarations
//====================================================================================================

class TerrainService : public Service
{
public:
    META_DECLARE_CLASS

    static const u16 kID = 2;

    TerrainService();
    ~TerrainService();
    
    void Initialize(GraphicsSystem& graphicsSystem, Camera& camera);
    virtual void Terminate();

    // Must be called between BeginRender() and EndRender()
    void Update();

    void SetCamera(Camera& camera);

private:
    virtual bool OnSubscribe(GameObjectHandle handle);

private:
    GraphicsSystem* mpGraphicsSystem;
    Camera* mpCamera;   // Use this view matrix to render the terrain
};

#endif // #ifndef INCLUDED_ENGINE_RENDERSERVICE_H