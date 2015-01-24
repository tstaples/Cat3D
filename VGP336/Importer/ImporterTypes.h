#ifndef INCLUDED_IMPORTERTYPES_H
#define INCLUDED_IMPORTERTYPES_H

#include <Engine.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct LocalMesh
{
    Mesh::Vertex* mpVertices;
    u16* mpIndices;
    u32 mNumVerts;
    u32 mNumIndices;

    size_t GetSize() const
    {
        return (sizeof(Mesh::Vertex) * mNumVerts +
                sizeof(u16) * mNumIndices);
    }
};

#endif