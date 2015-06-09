#ifndef INCLUDED_ASSETLOADER_H
#define INCLUDED_ASSETLOADER_H

//====================================================================================================
// Filename:	AssetLoader.h
// Created by:	Tyler Staples
// Description:	Class for loading native resource types
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "TextureManager.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

class Mesh;
class MeshBuffer;
class Model;
class GraphicsSystem;
class SerialReader;
class Texture;

//====================================================================================================
// Class Declaration
//====================================================================================================

class AssetLoader
{
    typedef std::vector<std::string> StringVec;
public:
    AssetLoader();

    void Initialize(GraphicsSystem& gs);
    void Terminate();
    
    bool LoadModel(const wchar_t* pFilename, Model& model);
    bool LoadMesh(const wchar_t* pFilename, Mesh& mesh, MeshBuffer& meshBuffer, std::vector<Texture*>& textures);

private:
    bool LoadCatmFile(const wchar_t* pFilename, Model& model);
    
    void LoadMeshes(SerialReader& reader, Model& model);
    void LoadTextures(const StringVec& paths, std::vector<Texture*>& textures);
    void LoadTexturesPaths(const wchar_t* pModelPath, SerialReader& reader, StringVec& paths);
    void LoadBones(SerialReader& reader, Model& model);
    void LoadBoneWeights(SerialReader& reader, Model& model);
    void LinkBones(Model& model);
    void LoadAnimations(SerialReader& reader, Model& model);

    void LoadSingleMesh(SerialReader& reader, Mesh& mesh, MeshBuffer& meshBuffer);

private:
    NONCOPYABLE(AssetLoader);

    GraphicsSystem* mpGraphicsSystem;
    TextureManager mTextureManager;
};

#endif