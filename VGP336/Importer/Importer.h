#ifndef INCLUDED_IMPORTER_H
#define INCLUDED_IMPORTER_H

#include <Engine.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <memory>

// Assimp types
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


class Importer
{
public:
    Importer();

    bool Load(const char* inputFile, f32 scale);
    bool Output(const char* outputFile);

private:
    bool Init();

    void CopyVertexData(const AIMeshPtr& aimesh, f32 scale, MeshPtr& mesh);
    void CopyIndexData(const AIMeshPtr& aimesh, MeshPtr& mesh);

    void CopyPosition(const aiVector3D& fromV, Mesh::Vertex& toV, f32 scale);
    void CopyNormal(const aiVector3D& fromV, Mesh::Vertex& toV);
    void CopyIndex(const aiFace& face, IndexList::iterator& indexIter);


private:
    Assimp::Importer mImporter;
    u32 mFlags;
  
    Meshes mMeshes;
};


#endif // #ifndef INCLUDED_IMPORTER_H