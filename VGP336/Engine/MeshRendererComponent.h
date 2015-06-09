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

    const char* GetTexturePath() const      { return mTexturePath.c_str(); }
    Texture& GetTexture()                   { return mTexture; }

private:
    friend class RenderService;

    Texture mTexture;

    // TODO: support multiple textures
    //std::vector<Texture*> mTextures;
    //u32 mTextureIndex; // index of selected texture

    // Note: Keep this as the last member so if it grows/shrinks it doesn't alter
    // the offset of other members.
    std::string mTexturePath; // TODO: store array of these
};

//====================================================================================================
// Typedefs
//====================================================================================================

//typedef MemHandle<MeshRendererComponent> MeshRendererHandle;

#endif // #ifndef INCLUDED_ENGINE_MESHRENDERERCOMPONENT_H