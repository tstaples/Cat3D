#include "Precompiled.h"
#include "MeshComponent.h"

#include "MeshBuilder.h"

META_CLASS_BEGIN(MeshComponent)
META_FIELD_BEGIN
    META_FIELD(mFilter, "Filter")
META_FIELD_END
META_CLASS_END

MeshComponent::MeshComponent()
    : mFilter(eFilters::Cube)
    , mFilterModified(false)
{
    // Default to cube
    MeshBuilder::CreateCube(mMesh);
}

//----------------------------------------------------------------------------------------------------

MeshComponent::~MeshComponent()
{
    // TODO: have this managed by some system
    mMesh.Destroy();
    mMeshBuffer.Terminate();
}

//----------------------------------------------------------------------------------------------------

void MeshComponent::Update(f32 deltaTime)
{
    if (IsDirty())
    {
        mMesh.Destroy();
        switch (mFilter)
        {
        case MeshComponent::Cube:
            MeshBuilder::CreateCube(mMesh);
            break;
        case MeshComponent::Quad:
            MeshBuilder::CreateQuad(mMesh);
            break;
        case MeshComponent::Sphere:
            MeshBuilder::CreateSphere(mMesh, 20u, 20u);
            break;
        case MeshComponent::Cylinder:
            MeshBuilder::CreateCylinder(mMesh, 1.0f, 20u, 20u);
            break;
        case MeshComponent::Torus:
            MeshBuilder::CreateTorus(mMesh, 5.0f, 3.0f, 20u, 20u);
            break;
        default:
            // TODO: support custom
            //MeshBuilder::GenerateMesh(mMesh, mMesh.GetVertices(), mMesh.GetVertexCount(), mMesh.GetIndices(), mMesh.GetIndexCount());
            break;
        }
        mFilterModified = true;
    }
}