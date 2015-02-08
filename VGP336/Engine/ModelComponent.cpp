#include "Precompiled.h"
#include "ModelComponent.h"

ModelComponent::ModelComponent()
    : mModel(nullptr)
{
}

//----------------------------------------------------------------------------------------------------

ModelComponent::~ModelComponent()
{
    SafeRelease(mModel);
}