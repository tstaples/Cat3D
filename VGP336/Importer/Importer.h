#ifndef INCLUDED_IMPORTER_H
#define INCLUDED_IMPORTER_H

#include "ImporterTypes.h"

class Importer
{
public:
    Importer();

    bool Load(const char* inputFile, f32 scale);
    const Meshes& GetMeshes() const { return mMeshes; }

private:
    bool Init();

    void CopyVertexData(const aiMesh& aimesh, f32 scale, MeshPtr& mesh);
    void CopyIndexData(const aiMesh& aimesh, MeshPtr& mesh);

    // Helpers to convert assimp formats to native
    Math::Vector3 ToV3(const aiVector3D& v);
    Color ToColor(const aiColor4D& c);

private:
    Assimp::Importer mImporter;
    u32 mFlags;
  
    Meshes mMeshes;
};


#endif // #ifndef INCLUDED_IMPORTER_H