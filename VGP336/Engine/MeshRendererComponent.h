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
#include "MeshRenderer.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

class MeshRendererComponent : public Component
{
public:
    META_DECLARE_CLASS

    MeshRendererComponent();
    ~MeshRendererComponent();

private:
    // TODO
    // Could hold data for mesh renderer instead
    // ie. texture/materials etc.
    // then RenderService can use that
};

//====================================================================================================
// Typedefs
//====================================================================================================

//typedef MemHandle<MeshRendererComponent> MeshRendererHandle;

#endif // #ifndef INCLUDED_ENGINE_MESHRENDERERCOMPONENT_H