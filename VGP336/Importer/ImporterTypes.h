#ifndef INCLUDED_IMPORTERTYPES_H
#define INCLUDED_IMPORTERTYPES_H

#include <Engine.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <memory>

// Local types
//typedef std::vector<Mesh::Vertex> NativeVertList;
//typedef std::vector<u16> IndexList;

struct LocalMesh
{
    Mesh::Vertex* mpVertices;
    u16* mpIndices;
    u32 mNumVerts;
    u32 mNumIndices;
    //NativeVertList mVertices;
    //IndexList mIndices;

    size_t GetSize() const
    {
        return (sizeof(Mesh::Vertex) * mNumVerts +
                sizeof(u16) * mNumIndices);
    }
};

//typedef std::unique_ptr<LocalMesh> MeshPtr;

#endif