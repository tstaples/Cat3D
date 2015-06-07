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
#include "TextureManager.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

class TerrainComponent : public Component
{
public:
    META_DECLARE_CLASS

    TerrainComponent();
    ~TerrainComponent();

    const Terrain& GetTerrain() const { return mTerrain; }
    Terrain& GetTerrain() { return mTerrain; }

    u32 GetWidth() const { return mLength; }
    u32 GetLength() const { return mWidth; }
    f32 GetMaxHeight() const { return mMaxHeight; }
    const char* GetHeightmapPath() const { return mHeightmapPath.c_str(); }

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
    u32 mNumLayers; // Temp: used to know how many of the layers are valid
    Terrain::Layer mLayer0;
    Terrain::Layer mLayer1;
};

//====================================================================================================
// Typedefs
//====================================================================================================

//typedef MemHandle<TransformComponent> TransformHandle;

#endif // #ifndef INCLUDED_ENGINE_TERRAINCOMPONENT_H