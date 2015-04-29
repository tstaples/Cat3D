//====================================================================================================
// Filename:	RenderService.h
// Created by:	Tyler Staples
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"
#include "RenderService.h"

#include "GraphicsSystem.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

RenderService::RenderService()
    : mpGraphicsSystem(nullptr)
    , mpGameObjectRepo(nullptr)
    , mpTransformRepo(nullptr)
    , mpModelRepo(nullptr)
{
}

//----------------------------------------------------------------------------------------------------

RenderService::~RenderService()
{
}

//----------------------------------------------------------------------------------------------------

void RenderService::Initialize(GraphicsSystem& graphicsSystem, 
                    GameObjectRepository& gameObjectRepo, 
                    TransformRepository& transformRepo, 
                    ModelRepository& modelRepo,
                    Camera& camera)
{
    mpGraphicsSystem    = &graphicsSystem;
    mpGameObjectRepo    = &gameObjectRepo;
    mpTransformRepo     = &transformRepo;
    mpModelRepo         = &modelRepo;

    mRenderer.Initialize(*mpGraphicsSystem);
    mRenderer.SetCamera(camera);
}

//----------------------------------------------------------------------------------------------------

void RenderService::Terminate()
{
    mpGraphicsSystem    = nullptr;
    mpGameObjectRepo    = nullptr;
    mpTransformRepo     = nullptr;
    mpModelRepo         = nullptr;

    mRenderer.Terminate();
}

//----------------------------------------------------------------------------------------------------

void RenderService::Update()
{
    // Iterate through all the subscribers and render them
    for (auto subscriberInfo : mSubscribers)
    {
        RenderInfo& renderInfo = subscriberInfo.second;

        // Get the transform and model components
        TransformComponent& transformComponent   = mpTransformRepo->GetItem(renderInfo.transformId);
        ModelComponent& modelComponent           = mpModelRepo->GetItem(renderInfo.modelId);

        // Cache the model and the transform
        const Math::Matrix& transform   = transformComponent.GetTransform();
        const Model& model              = *modelComponent.GetModel();

        // Render the model at the specified transform
        model.Render(mRenderer, transform);
    }
}

//----------------------------------------------------------------------------------------------------

void RenderService::OnSubscribe(ID ObjId, RenderInfo& info)
{
    // Get the object associated with the ID from the GameObject repository
    // and populate info with its ID and model ID.
    GameObject& object  = mpGameObjectRepo->GetItem(ObjId);
    //info.transformId    = object.GetComponentID(Meta::TransformComponentType);
    //info.modelId        = object.GetComponentID(Meta::ModelComponentType);
}