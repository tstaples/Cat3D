#ifndef INCLUDED_ENGINE_MESHCOMPONENT_H
#define INCLUDED_ENGINE_MESHCOMPONENT_H

//====================================================================================================
// Filename:	MeshComponent.h
// Created by:	Tyler Staples
// Description: Class representing Mesh Component.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Component.h"
#include "Mesh.h"
#include "MeshBuffer.h"
#include "RenderService.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

class MeshComponent : public Component
{
public:
    META_DECLARE_CLASS

    enum eFilters : u32
    {
        Cube,
        Quad,
        Sphere,
        Cylinder,
        Torus
    };

    MeshComponent();
    ~MeshComponent();

    u32 GetFilter() const           { return mFilter; }
    Mesh& GetMesh()                 { return mMesh; }
    MeshBuffer& GetMeshBuffer()     { return mMeshBuffer; }
    const char* GetCurrentMeshPath() const { return mMeshPaths[mFilter].c_str(); }

private:
    friend RenderService;

    u32 mFilter;
    bool mFilterModified;

    Mesh mMesh;
    MeshBuffer mMeshBuffer;

    std::vector<std::string> mMeshPaths;
};

//====================================================================================================
// Typedefs
//====================================================================================================

//typedef MemHandle<MeshComponent> MeshHandle;

#endif // #ifndef INCLUDED_ENGINE_MESHCOMPONENT_H