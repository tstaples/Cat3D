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

        TerrainComponent* terrainComponent = nullptr;
        gameObject->GetComponent(terrainComponent);

        // Render the terrain
        terrainComponent->mTerrain.Render();
    }
}

//----------------------------------------------------------------------------------------------------

bool TerrainService::OnSubscribe(GameObjectHandle handle)
{
    GameObject* gameObject = handle.Get();

    // TODO: set position based on transform component
    TerrainComponent* terrainComponent = nullptr;
    gameObject->GetComponent(terrainComponent);

    // Init the terrain
    const char* heightMapPath = terrainComponent->mHeightmapPath.c_str();
    const u32 width = terrainComponent->mWidth;
    const u32 length = terrainComponent->mLength;
    const f32 maxheight = terrainComponent->mMaxHeight;
    Terrain& terrain = terrainComponent->mTerrain;
    terrain.Initialize(*mpGraphicsSystem, heightMapPath, width, length, maxheight);

    // Set the camera
    ASSERT(mpCamera != nullptr, "[TerrainService] Camera not initialized");
    terrain.SetCamera(*mpCamera);

    // Set layers
    // Temp populate layer struct here. TODO: stored in json file
    Terrain::Layer& layer0 = terrainComponent->mLayer0;
    layer0.layerIndex = 0;
    layer0.frequency = 21.0f;
    layer0.minHeight = 0;
    layer0.maxHeight = 30;
    layer0.texturePath = "../Data/Heightmaps/default_heightmap.raw";
    Texture* texture = mpTextureManager->GetResource(layer0.texturePath.c_str()); 
    terrain.SetLayer(texture, layer0);

    return true;
}

//----------------------------------------------------------------------------------------------------

void TerrainService::SetCamera(Camera& camera)
{
    mpCamera = &camera;
}