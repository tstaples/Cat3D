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

class RenderService : public Service
{
public:
    META_DECLARE_CLASS

    static const u16 kID = 0;

    RenderService();
    ~RenderService();
    
    void Initialize(GraphicsSystem& graphicsSystem, Camera& camera, AssetLoader& assetLoader);
    virtual void Terminate();

    // Must be called between BeginRender() and EndRender()
    void Update();

    void SetCamera(Camera& camera);

private:
    virtual bool OnSubscribe(GameObjectHandle handle);

    // TODO: Use texture manager
    bool LoadMesh(MeshComponent* meshComponent, MeshRendererComponent* meshRendererComponent);
    bool LoadTexture(MeshRendererComponent* meshRendererComponent);

private:
    GraphicsSystem* mpGraphicsSystem;
    MeshRenderer mRenderer;
    AssetLoader* mAssetLoader;
};

#endif // #ifndef INCLUDED_ENGINE_RENDERSERVICE_H