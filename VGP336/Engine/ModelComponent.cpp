#include "Precompiled.h"
#include "ModelComponent.h"

#include "AssetLoader.h"

#include <json/json.h>

ModelComponent::ModelComponent()
    : mModel(nullptr)
{
}

//----------------------------------------------------------------------------------------------------

ModelComponent::~ModelComponent()
{
    SafeRelease(mModel);
}

//----------------------------------------------------------------------------------------------------

void ModelComponent::Load(Json::Value& properties)
{
    Json::Value filename = properties.get("Filename", "");

}