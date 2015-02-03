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

class Model;
class GraphicsSystem;
class SerialReader;

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

private:
    bool LoadCatmFile(const wchar_t* pFilename, Model& model);
    
    void LoadMeshes(SerialReader& reader, Model& model);
    void LoadTextures(const StringVec& paths, Model& model);
    void LoadTexturesPaths(const wchar_t* pModelPath, SerialReader& reader, StringVec& paths);
    void LoadBones(SerialReader& reader, Model& model);
    void LoadBoneWeights(SerialReader& reader, Model& model);

private:
    GraphicsSystem* mpGraphicsSystem;
    TextureManager mTextureManager;
};

#endif