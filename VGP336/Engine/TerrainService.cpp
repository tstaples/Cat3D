//====================================================================================================
// Filename:	TerrainService.h
// Created by:	Tyler Staples
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"
#include "TerrainService.h"

#include "Camera.h"
#include "GraphicsSystem.h"
#include "IO.h"
#include "MemHandle.h"
#include "MemoryPool.h"
#include "TransformComponent.h"
#include "TerrainComponent.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

META_CLASS_BEGIN(TerrainService)
META_DEPENDENCIES_BEGIN
    META_DEPENDENCY("TerrainComponent", "Component")
META_DEPENDENCIES_END
META_CLASS_END

namespace
{
    void InitTerrain(TerrainComponent* terrainComponent, Math::Vector3 pos, GraphicsSystem& gs)
    {
        const char* heightMapPath = terrainComponent->GetHeightmapPath();
        const u32 width = terrainComponent->GetWidth();
        const u32 length = terrainComponent->GetLength();
        const f32 maxheight = terrainComponent->GetMaxHeight();
        Terrain& terrain = terrainComponent->GetTerrain();
        terrain.Initialize(gs, heightMapPath, width, length, maxheight, pos);
    }
}

TerrainService::TerrainService()
    : Service("TerrainService", kID)
    , mpGraphicsSystem(nullptr)
    , mpCamera(nullptr)
{
}

//----------------------------------------------------------------------------------------------------

TerrainService::~TerrainService()
{
}

//----------------------------------------------------------------------------------------------------

void TerrainService::Initialize(GraphicsSystem& graphicsSystem, Camera& camera, TextureManager& textureManager)
{
    mpGraphicsSystem = &graphicsSystem;
    mpCamera = &camera;
    mpTextureManager = &textureManager;
}

//----------------------------------------------------------------------------------------------------

void TerrainService::Terminate()
{
    Service::Terminate();
    mpGraphicsSystem = nullptr;
}

//----------------------------------------------------------------------------------------------------

void TerrainService::Update()
{
    // Iterate through all the subscribers and render them
    Subscribers::iterator it = mSubscribers.begin();
    for (it; it != mSubscribers.end(); ++it)
    {
        GameObject* gameObject = it->Get();

        TransformComponent* transformComponent = nullptr;
        TerrainComponent* terrainComponent = nullptr;
        gameObject->GetComponent(transformComponent);
        gameObject->GetComponent(terrainComponent);

        if (terrainComponent->IsDirty())
        {
            // Re-create the terrain with the desired settings
            Terrain& terrain = terrainComponent->mTerrain;
            terrain.Terminate();
            InitTerrain(terrainComponent, transformComponent->GetPosition(), *mpGraphicsSystem);
            
            terrain.SetCamera(*mpCamera);

            // TODO: remove duplication of this once layers can be set properly
            Terrain::Layer& layer0 = terrainComponent->mLayer0;
            layer0.layerIndex = 0;
            layer0.frequency = 21.0f;
            layer0.minHeight = 0;
            layer0.maxHeight = 30;
            layer0.texturePath = "../Data/Images/grass.jpg";
            Texture* texture = mpTextureManager->GetResource(layer0.texturePath.c_str()); 
            terrain.SetLayer(texture, layer0);

            terrainComponent->SetIsDirty(false);
        }

        // Render the terrain
        terrainComponent->mTerrain.Render(transformComponent->GetTransform());
    }
}

//----------------------------------------------------------------------------------------------------

bool TerrainService::OnSubscribe(GameObjectHandle handle)
{
    GameObject* gameObject = handle.Get();

    // TODO: set position based on transform component
    TransformComponent* transformComponent = nullptr;
    TerrainComponent* terrainComponent = nullptr;
    gameObject->GetComponent(transformComponent);
    gameObject->GetComponent(terrainComponent);

    // Init the terrain
    InitTerrain(terrainComponent, transformComponent->GetPosition(), *mpGraphicsSystem);

    // Set the camera
    ASSERT(mpCamera != nullptr, "[TerrainService] Camera not initialized");
    Terrain& terrain = terrainComponent->GetTerrain();
    terrain.SetCamera(*mpCamera);

    // Set layers
    // Temp populate layer struct here. TODO: stored in json file
    Terrain::Layer& layer0 = terrainComponent->mLayer0;
    layer0.layerIndex = 0;
    layer0.frequency = 21.0f;
    layer0.minHeight = 0;
    layer0.maxHeight = 30;
    layer0.texturePath = "../Data/Images/grass.jpg"; // temp
    Texture* texture = mpTextureManager->GetResource(layer0.texturePath.c_str()); 
    terrain.SetLayer(texture, layer0);

    return true;
}

//----------------------------------------------------------------------------------------------------

void TerrainService::SetCamera(Camera& camera)
{
    mpCamera = &camera;
}