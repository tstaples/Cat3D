#ifndef INCLUDED_IMPORTERTYPES_H
#define INCLUDED_IMPORTERTYPES_H

#include <Engine.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <memory>

typedef std::unique_ptr<const aiScene> AIScenePtr;
typedef std::unique_ptr<aiMesh> AIMeshPtr;

// Local types
typedef std::vector<Mesh::Vertex> NativeVertList;
typedef std::vector<u16> IndexList;

struct LocalMesh
{
    NativeVertList mVertices;
    IndexList mIndices;
};

typedef std::unique_ptr<LocalMesh> MeshPtr;
typedef std::vector<MeshPtr> Meshes;

#endif