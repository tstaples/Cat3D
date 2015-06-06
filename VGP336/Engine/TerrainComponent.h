#ifndef INCLUDED_ENGINE_TERRAINCOMPONENT_H
#define INCLUDED_ENGINE_TERRAINCOMPONENT_H

//====================================================================================================
// Filename:	TerrainComponent.h
// Created by:	Tyler Staples
// Description: Class representing a Terrain Component.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Component.h"
#include "EngineMath.h"
#include "Terrain.h"

struct TerrainLayer
{
    u32 layer;
    f32 minHeight;
    f32 maxHeight;
    f32 frequency;
    Texture mTexture;
    std::string mTexturePath;
};

//====================================================================================================
// Class Declarations
//====================================================================================================

class TerrainComponent : public Component
{
public:
    META_DECLARE_CLASS

    TerrainComponent();
    ~TerrainComponent();

private:
    friend class TerrainService;

    Terrain mTerrain;
    
    // Exposed to meta system
    u32 mWidth;
    u32 mLength;
    f32 mMaxHeight;
    std::string mHeightmapPath;

    // TODO: make array
    // For now just support up to 3 different texture layers until meta system can support arrays
    //TerrainLayer mLayer1;
};

//====================================================================================================
// Typedefs
//====================================================================================================

//typedef MemHandle<TransformComponent> TransformHandle;

#endif // #ifndef INCLUDED_ENGINE_TERRAINCOMPONENT_H