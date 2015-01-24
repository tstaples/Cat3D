#ifndef INCLUDED_ASSETLOADER_H
#define INCLUDED_ASSETLOADER_H

class Model;
class GraphicsSystem;
class SerialReader;

class AssetLoader
{
    typedef std::vector<std::string> StringVec;
public:
    static bool LoadModel(const char* pFilename, GraphicsSystem& gs, Model& model);

private:
    static bool LoadCatmFile(const char* pFilename, GraphicsSystem& gs, Model& model);
    
    static bool LoadMeshes(GraphicsSystem& gs, SerialReader& reader, Model& model);
    static bool LoadTextures(GraphicsSystem& gs, const StringVec& paths, Model& model);
    static bool LoadTexturesPaths(const char* pModelPath, SerialReader& reader, StringVec& paths);
};

#endif