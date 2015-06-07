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