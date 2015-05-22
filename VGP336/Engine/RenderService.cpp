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
#include "MemHandle.h"
#include "MemoryPool.h"
#include "MeshBuilder.h"
#include "MeshComponent.h"
#include "MeshRendererComponent.h"
#include "TransformComponent.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

RenderService::RenderService()
    : mpGraphicsSystem(nullptr)
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

        gameObject->GetComponent(transformComponent);
        gameObject->GetComponent(meshComponent);

        MeshBuffer& meshBuffer = meshComponent->GetMeshBuffer();
        Math::Matrix transform = transformComponent->GetTransform();

        // Check if the component was modified
        if (meshComponent->mFilterModified)
        {
            // Update the MeshBuffer
            Mesh& mesh = meshComponent->GetMesh();
            // Recreate the mesh buffer
            meshBuffer.Terminate();
            meshBuffer.Initialize(*mpGraphicsSystem, mesh.GetVertexFormat(), mesh);
            meshComponent->mFilterModified = false;
        }
        mRenderer.Render(meshBuffer, transform);
    }
}

//----------------------------------------------------------------------------------------------------

void RenderService::OnSubscribe(GameObjectHandle handle)
{
    GameObject* gameObject = handle.Get();
    TransformComponent* transformComponent = nullptr;
    MeshComponent* meshComponent = nullptr;
    //MeshRendererComponent* meshRendererComponent = nullptr;
    
    // TODO: throw exception instead of asserting
    gameObject->GetComponent(transformComponent);
    gameObject->GetComponent(meshComponent);
    //gameObject->GetComponent(meshRendererComponent); // Not needed for now

    // Init the mesh buffer
    Mesh& mesh = meshComponent->GetMesh();
    MeshBuffer& meshBuffer = meshComponent->GetMeshBuffer();
    meshBuffer.Initialize(*mpGraphicsSystem, mesh.GetVertexFormat(), mesh);
}