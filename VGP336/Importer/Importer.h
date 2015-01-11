#ifndef INCLUDED_IMPORTER_H
#define INCLUDED_IMPORTER_H

#include "ImporterTypes.h"

class Importer
{
public:
    Importer();

    bool Load(const char* inputFile, f32 scale);

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