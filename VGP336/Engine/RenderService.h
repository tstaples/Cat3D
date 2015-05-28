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

#include "GameObject.h"
#include "MeshRenderer.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

class Camera;
class GraphicsSystem;

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
    
    void Initialize(GraphicsSystem& graphicsSystem, Camera& camera);
    void Terminate();

    // Must be called between BeginRender() and EndRender()
    void Update();

private:
    virtual bool OnSubscribe(GameObjectHandle handle);

private:
    GraphicsSystem* mpGraphicsSystem;
    MeshRenderer mRenderer;
};

#endif // #ifndef INCLUDED_ENGINE_RENDERSERVICE_H