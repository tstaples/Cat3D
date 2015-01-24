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
    
    bool LoadModel(const char* pFilename, Model& model);

private:
    bool LoadCatmFile(const char* pFilename, Model& model);
    
    bool LoadMeshes(SerialReader& reader, Model& model);
    bool LoadTextures(const StringVec& paths, Model& model);
    bool LoadTexturesPaths(const char* pModelPath, SerialReader& reader, StringVec& paths);

private:
    GraphicsSystem* mpGraphicsSystem;
    TextureManager mTextureManager;
};

#endif