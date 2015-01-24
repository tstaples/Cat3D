#ifndef INCLUDED_ENGINE_MODEL_H
#define INCLUDED_ENGINE_MODEL_H

#include "Common.h"

class Mesh;
class MeshBuffer;
class MeshRenderer;
class Texture;
class GraphicsSystem;
class SerialReader;

class Model
{
public:
    Model() {}
    ~Model() {}

    void Load(GraphicsSystem& gs, const char* pFilename);
    void Unload();

    void Render(MeshRenderer& renderer);

private:
    NONCOPYABLE(Model);

public:
	std::vector<Mesh*> mMeshes;
	std::vector<MeshBuffer*> mMeshBuffers;
    std::vector<Texture*> mTextures;
};

#endif