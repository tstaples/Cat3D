//====================================================================================================
// Filename:	RenderService.h
// Created by:	Tyler Staples
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"
#include "RenderService.h"

#include "Camera.h"
#include "GraphicsSystem.h"
#include "IO.h"
#include "MemHandle.h"
#include "MemoryPool.h"
#include "MeshBuilder.h"
#include "MeshComponent.h"
#include "MeshRendererComponent.h"
#include "TransformComponent.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

META_CLASS_BEGIN(RenderService)
META_DEPENDENCIES_BEGIN
    META_DEPENDENCY("TransformComponent", "Component")
    META_DEPENDENCY("MeshComponent", "Component")
META_DEPENDENCIES_END
META_CLASS_END

RenderService::RenderService()
    : Service("RenderService", kID)
    , mpGraphicsSystem(nullptr)
{
}

//----------------------------------------------------------------------------------------------------

RenderService::~RenderService()
{
}

//----------------------------------------------------------------------------------------------------

void RenderService::Initialize(GraphicsSystem& graphicsSystem, Camera& camera)
{
    mpGraphicsSystem = &graphicsSystem;

    mRenderer.Initialize(*mpGraphicsSystem);
    mRenderer.SetCamera(camera);
}

//----------------------------------------------------------------------------------------------------

void RenderService::Terminate()
{
    mpGraphicsSystem = nullptr;
    mRenderer.Terminate();
}

//----------------------------------------------------------------------------------------------------

void RenderService::Update()
{
    // Iterate through all the subscribers and render them
    Subscribers::iterator it = mSubscribers.begin();
    for (it; it != mSubscribers.end(); ++it)
    {
        GameObject* gameObject = it->Get();
        TransformComponent* transformComponent = nullptr;
        MeshComponent* meshComponent = nullptr;
        MeshRendererComponent* meshRendererComponent = nullptr;

        gameObject->GetComponent(transformComponent);
        gameObject->GetComponent(meshComponent);
        if (gameObject->FindComponent(meshRendererComponent))
        {
            // Destroy old texture and load new one
            Texture& texture = meshRendererComponent->GetTexture();
            std::wstring texturePath = IO::CharToWChar(meshRendererComponent->GetTexturePath());
            texture.Terminate();
            texture.Initialize(*mpGraphicsSystem, texturePath.c_str());
            mRenderer.SetTexture(texture);
        }

        MeshBuffer& meshBuffer = meshComponent->GetMeshBuffer();
        Math::Matrix transform = transformComponent->GetTransform();

        // Check if the component was modified
        if (meshComponent->IsDirty())
        {
            Mesh& mesh = meshComponent->GetMesh();

            // Recreate the mesh buffer
            meshBuffer.Terminate();
            meshBuffer.Initialize(*mpGraphicsSystem, mesh.GetVertexFormat(), mesh);
            meshComponent->mFilterModified = false;
        }
        mRenderer.Render(meshBuffer, transform);

        // Temp: uncheck flag here until a more suitable location is found
        meshComponent->SetIsDirty(false);
    }
}

//----------------------------------------------------------------------------------------------------

bool RenderService::OnSubscribe(GameObjectHandle handle)
{
    GameObject* gameObject = handle.Get();
    TransformComponent* transformComponent = nullptr;
    MeshComponent* meshComponent = nullptr;
    MeshRendererComponent* meshRendererComponent = nullptr;
    
    // Ensure the GameObject has the required components
    if (!gameObject->FindComponent(transformComponent) ||
        !gameObject->FindComponent(meshComponent))
    {
        return false;
    }

    // Init the mesh buffer
    Mesh& mesh = meshComponent->GetMesh();
    MeshBuffer& meshBuffer = meshComponent->GetMeshBuffer();
    meshBuffer.Initialize(*mpGraphicsSystem, mesh.GetVertexFormat(), mesh);

    // MeshRenderer is not a dependency, so allow it to not exist
    if (gameObject->FindComponent(meshRendererComponent))
    {
        // Init and set the texture
        Texture& texture = meshRendererComponent->GetTexture();
        std::wstring texturePath = IO::CharToWChar(meshRendererComponent->GetTexturePath());
        texture.Initialize(*mpGraphicsSystem, texturePath.c_str()); 
        mRenderer.SetTexture(texture);
    }
    return true;
}

//----------------------------------------------------------------------------------------------------

void RenderService::SetCamera(Camera& camera)
{
    mRenderer.SetCamera(camera);
}