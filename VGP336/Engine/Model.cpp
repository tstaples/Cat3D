#include "Precompiled.h"
#include "Model.h"
#include "Mesh.h"
#include "MeshBuffer.h"

void Model::Destroy()
{
    for (auto mesh : mMeshes)
    {
        mesh->Destroy();
    }
    mMeshes.clear();

    for (auto meshBuffer : mMeshBuffers)
    {
        meshBuffer->Terminate();
    }
    mMeshBuffers.clear();
}