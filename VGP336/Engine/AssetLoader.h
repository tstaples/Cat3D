#ifndef INCLUDED_ASSETLOADER_H
#define INCLUDED_ASSETLOADER_H

class Model;
class GraphicsSystem;

class AssetLoader
{
public:
    static bool LoadModel(const char* pFilename, GraphicsSystem& gs, Model& model);

private:
    static bool LoadCatmFile(const char* pFilename, GraphicsSystem& gs, Model& model);
};

#endif