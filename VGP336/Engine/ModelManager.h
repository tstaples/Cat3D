#ifndef INCLUDED_ENGINE_MODELMANAGER_H
#define INCLUDED_ENGINE_MODELMANAGER_H

//====================================================================================================
// Filename:	ModelManager.h
// Created by:	Tyler Staples
// Description:	Class for managing model resources.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "ResourceManager.h"
#include "Model.h"
#include "AssetLoader.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

class ModelManager : public ResourceManager<Model>
{
public:
    ModelManager();
    ~ModelManager();

    void Initialize(GraphicsSystem& gs);

private:
    virtual void InitResource(Model* res, const wchar_t* pFilename);
    virtual void CleanupResource(Model* res);

private:
    GraphicsSystem* mpGraphicsSystem;

    AssetLoader mAssetLoader;
};

#endif // #ifndef INCLUDED_ENGINE_TEXTUREMANAGER_H