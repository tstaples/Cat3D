#include "Precompiled.h"
#include "ModelComponent.h"

#include "AssetLoader.h"

#include <json/json.h>

ModelComponent::ModelComponent(GameObject* gameObject)
    : Component(gameObject)
    , mModel(nullptr)
{
}

//----------------------------------------------------------------------------------------------------

ModelComponent::~ModelComponent()
{
    SafeRelease(mModel);
}