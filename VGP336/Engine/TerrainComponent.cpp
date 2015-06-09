#include "Precompiled.h"
#include "TerrainComponent.h"

#include "GameWorld.h"

META_CLASS_BEGIN(TerrainComponent)
META_FIELD_BEGIN
    META_FIELD(mWidth, "Width")
    META_FIELD(mLength, "Length")
    META_FIELD(mMaxHeight, "MaxHeight")
    META_FIELD(mHeightmapPath, "HeightmapPath")
META_FIELD_END
META_DEPENDENCIES_BEGIN
    META_DEPENDENCY("TerrainService", "Service")
META_DEPENDENCIES_END
META_CLASS_END

namespace
{
    const char* defaultHeightMapPath = "../Data/Heightmaps/default_heightmap.raw";
}

TerrainComponent::TerrainComponent()
    : mWidth(0)
    , mLength(0)
    , mMaxHeight(1.0f)
    , mHeightmapPath(defaultHeightMapPath)
{
}

//----------------------------------------------------------------------------------------------------

TerrainComponent::~TerrainComponent()
{
    mTerrain.Terminate();
}