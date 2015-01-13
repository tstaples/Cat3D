#ifndef INCLUDED_ENGINE_TEXTUREMANAGER_H
#define INCLUDED_ENGINE_TEXTUREMANAGER_H

//====================================================================================================
// Filename:	TextureManager.h
// Created by:	Tyler Staples
// Description:	Class for managing texture resources
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "ResourceManager.h"
#include "Texture.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

class TextureManager : public ResourceManager<Texture>
{
public:
    TextureManager();

    void Initialize(GraphicsSystem& gs);

private:
    virtual void InitResource(Texture* res, const wchar_t* pFilename);
    virtual void CleanupResource(Texture* res);

private:
    GraphicsSystem* mpGraphicsSystem;
};

#endif // #ifndef INCLUDED_ENGINE_TEXTUREMANAGER_H