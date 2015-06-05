#include "Precompiled.h"
#include "MeshComponent.h"

#include "MeshBuilder.h"

META_CLASS_BEGIN(MeshComponent)
META_FIELD_BEGIN
    META_FIELD(mFilter, "Filter")
META_FIELD_END
META_DEPENDENCIES_BEGIN
    META_DEPENDENCY("RenderService", "Service")
META_DEPENDENCIES_END
META_CLASS_END

MeshComponent::MeshComponent()
    : mFilter(eFilters::Cube)
    , mFilterModified(false)
{
    mMeshPaths = 
    {
        "../Data/Models/cube.catm"
    };
}

//----------------------------------------------------------------------------------------------------

MeshComponent::~MeshComponent()
{
    // TODO: have this managed by some system
    mMesh.Destroy();
    mMeshBuffer.Terminate();
}