#include "Precompiled.h"
#include "Model.h"
#include "Mesh.h"
#include "MeshBuffer.h"
#include "MeshRenderer.h"
#include "Texture.h"
#include "GraphicsSystem.h"

void Model::Load(GraphicsSystem& gs, const char* pFilename)
{


}

void Model::Unload()
{
    for (auto mesh : mMeshes)
    {
        mesh->Destroy();
        SafeDelete(mesh);
    }
    mMeshes.clear();

    for (auto meshBuffer : mMeshBuffers)
    {
        meshBuffer->Terminate();
    }
    mMeshBuffers.clear();

    for (auto tex : mTextures)
    {
        tex->Terminate();
    }
    mTextures.clear();
}

void Model::Render(MeshRenderer& renderer)
{
    const u32 meshBufferSize = mMeshBuffers.size();
    for (u32 i = 0; i < meshBufferSize; ++i)
    {              
        renderer.SetTexture(*mTextures[i]);
        renderer.Render(*mMeshBuffers[i], Math::Matrix::Identity());
    }
}