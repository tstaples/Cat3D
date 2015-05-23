#ifndef INCLUDED_ENGINE_MESHRENDERERCOMPONENT_H
#define INCLUDED_ENGINE_MESHRENDERERCOMPONENT_H

//====================================================================================================
// Filename:	MeshRendererComponent.h
// Created by:	Tyler Staples
// Description: Class representing Mesh Renderer Component.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Component.h"
#include "Texture.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

class MeshRendererComponent : public Component
{
public:
    META_DECLARE_CLASS

    MeshRendererComponent();
    ~MeshRendererComponent();

    const wchar_t* GetTexturePath() const   { return mTexturePath; }
    Texture& GetTexture()                   { return mTexture; }

private:
    Texture mTexture;

    // Note: Keep this as the last member so if it grows/shrinks it doesn't alter
    // the offset of other members.
    wchar_t mTexturePath[MAX_PATH];
};

//====================================================================================================
// Typedefs
//====================================================================================================

//typedef MemHandle<MeshRendererComponent> MeshRendererHandle;

#endif // #ifndef INCLUDED_ENGINE_MESHRENDERERCOMPONENT_H