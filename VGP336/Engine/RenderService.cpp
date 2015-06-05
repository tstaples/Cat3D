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
    , mAssetLoader(nullptr)
{
}

//----------------------------------------------------------------------------------------------------

RenderService::~RenderService()
{
}

//----------------------------------------------------------------------------------------------------

void RenderService::Initialize(GraphicsSystem& graphicsSystem, Camera& camera, AssetLoader& assetLoader)
{
    mpGraphicsSystem = &graphicsSystem;

    mRenderer.Initialize(*mpGraphicsSystem);
    mRenderer.SetCamera(camera);

    mAssetLoader = &assetLoader;
}

//----------------------------------------------------------------------------------------------------

void RenderService::Terminate()
{
    Service::Terminate();
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

        // Get the needed components
        gameObject->GetComponent(transformComponent);
        gameObject->GetComponent(meshComponent);
        bool hasMeshRenderer = gameObject->FindComponent(meshRendererComponent); // Not a dependency

        // Check if the component was modified
        if (meshComponent->IsDirty())
        {
            VERIFY(LoadMesh(meshComponent, meshRendererComponent), "[RenderService] Failed to load mesh");
        }
        else if (hasMeshRenderer && meshRendererComponent->IsDirty())
        {
            // Load new texture
            LoadTexture(meshRendererComponent);
        }

        // Render the mesh
        MeshBuffer& meshBuffer = meshComponent->GetMeshBuffer();
        Math::Matrix transform = transformComponent->GetTransform();
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
    bool hasMeshRenderer = gameObject->FindComponent(meshRendererComponent);

    // Init the mesh buffer
    Mesh& mesh = meshComponent->GetMesh();
    MeshBuffer& meshBuffer = meshComponent->GetMeshBuffer();
    if (LoadMesh(meshComponent, meshRendererComponent))
    {
       return true;
    }
    ASSERT(false, "[RenderService] Failed to load mesh");
    return false;
}

//----------------------------------------------------------------------------------------------------

void RenderService::SetCamera(Camera& camera)
{
    mRenderer.SetCamera(camera);
}

//----------------------------------------------------------------------------------------------------

bool RenderService::LoadMesh(MeshComponent* meshComponent, MeshRendererComponent* meshRendererComponent)
{
    bool hasMeshRenderer = (meshRendererComponent != nullptr);

    MeshBuffer& meshBuffer = meshComponent->GetMeshBuffer();
    Mesh& mesh = meshComponent->GetMesh();

    std::wstring meshPath = IO::CharToWChar(meshComponent->GetCurrentMeshPath());
    std::vector<Texture*> textures;

    // Load the mesh
    if (mAssetLoader->LoadMesh(meshPath.c_str(), mesh, meshBuffer, textures))
    {
        // Recreate the mesh buffer
        meshBuffer.Terminate();
        meshBuffer.Initialize(*mpGraphicsSystem, mesh.GetVertexFormat(), mesh);

        // Assign the textures to the mesh renderer if we have one
        if (hasMeshRenderer)
        {
            if (textures.size() > 0)
            {
                // Will hopefully support multiple textures one day
                meshRendererComponent->mTexture = *textures.front();
                meshRendererComponent->mTexturePath = ""; // TODO: store path when loading texture from asset loader
            }
            LoadTexture(meshRendererComponent);
        }
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------

bool RenderService::LoadTexture(MeshRendererComponent* meshRendererComponent)
{
    // Init and set the texture
    Texture& texture = meshRendererComponent->GetTexture();
    std::wstring texturePath = IO::CharToWChar(meshRendererComponent->GetTexturePath());

    texture.Terminate(); // Release any existing texture
    texture.Initialize(*mpGraphicsSystem, texturePath.c_str()); 
    mRenderer.SetTexture(texture);
    return true;
}