#include "Precompiled.h"

#include "Model.h"

#include "AnimationClip.h"
#include "Bone.h"
#include "EngineMath.h"
#include "GraphicsSystem.h"
#include "Mesh.h"
#include "MeshBuffer.h"
#include "MeshRenderer.h"
#include "Texture.h"

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
        tex->Release();
    }
    mTextures.clear();

    for (auto bone : mBones)
    {
        delete bone;
    }
    mBones.clear();

    for (auto animation : mAnimations)
    {
        delete animation;
    }
    mAnimations.clear();

    mBoneIndexMap.clear();
    mpRoot = nullptr;
}

void Model::Render(MeshRenderer& renderer, const Math::Matrix& transform) const
{
    const u32 meshBufferSize = mMeshBuffers.size();
    for (u32 i = 0; i < meshBufferSize; ++i)
    {              
        // Check that there is a corresponding texture for this mesh buffer
        if (mTextures.size() < i)
        {
            renderer.SetTexture(*mTextures[i]);
        }
        renderer.Render(*mMeshBuffers[i], transform);
    }
}