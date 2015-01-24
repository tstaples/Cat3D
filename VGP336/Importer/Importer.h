#ifndef INCLUDED_IMPORTER_H
#define INCLUDED_IMPORTER_H

#include "ImporterTypes.h"

class Importer
{
    typedef std::vector<Mesh*> Meshes;
    typedef std::vector<std::string> StringVec;
public:
    Importer();

    bool Load(const char* inputFile, f32 scale);
    const Meshes& GetMeshes() const { return mMeshes; }
    const StringVec& GetTexturePaths() const { return mTexturePaths; }

private:
    bool Init();

    void LoadMeshes(const aiScene& scene, f32 scale);
    void LoadMaterials(const aiScene& scene);

    void CopyVertexData(const aiMesh& aimesh, f32 scale, Mesh::Vertex* vertices);
    void CopyIndexData(const aiMesh& aimesh, u16* indices);

    // Helpers to convert assimp formats to native
    Math::Vector3 ToV3(const aiVector3D& v);
    Color ToColor(const aiColor4D& c);

private:
    Assimp::Importer mImporter;
    u32 mFlags;
  
    Meshes mMeshes;
    StringVec mTexturePaths;
};


#endif // #ifndef INCLUDED_IMPORTER_H