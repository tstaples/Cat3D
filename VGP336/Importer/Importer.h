#ifndef INCLUDED_IMPORTER_H
#define INCLUDED_IMPORTER_H

#include "ImporterTypes.h"

class Importer
{
    typedef std::vector<Mesh*> Meshes;
    typedef std::vector<std::string> StringVec;
    typedef std::vector<Bone*> BoneVec;
public:
    Importer();

    bool Load(const char* inputFile, f32 scale, u32 flags);
    void Cleanup();

    const Meshes& GetMeshes() const             { return mMeshes; }
    const StringVec& GetTexturePaths() const    { return mTexturePaths; }
    const BoneVec& GetBones() const             { return mBones; }

private:
    void LoadMeshes(const aiScene& scene, f32 scale);
    void LoadMaterials(const aiScene& scene);
    void LoadAnimations(const aiScene& scene);

    void CopyVertexData(const aiMesh& aimesh, f32 scale, Mesh::Vertex* vertices);
    void CopyIndexData(const aiMesh& aimesh, u16* indices);
    void LoadBoneData(const aiMesh& aimesh, Mesh* mesh);


    // Helpers to convert assimp formats to native
    Math::Vector3 ToV3(const aiVector3D& v);
    Color ToColor(const aiColor4D& c);
    Bone* BuildSkeleton(aiNode& ainode, Bone* parent);

private:
    Assimp::Importer mImporter;
    u32 mFlags;
  
    Meshes mMeshes;
    StringVec mTexturePaths;

    Bone* mpRoot;
    std::vector<Bone*> mBones;
    std::map<std::string, u32> mBoneIndexMap;
};


#endif // #ifndef INCLUDED_IMPORTER_H