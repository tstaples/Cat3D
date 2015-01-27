#ifndef INCLUDED_ENGINE_MODEL_H
#define INCLUDED_ENGINE_MODEL_H

#include "Common.h"

namespace Math
{
    struct Matrix;
}

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

    void Unload();

    // Renders this model at the specified transform
    void Render(MeshRenderer& renderer, const Math::Matrix& transform) const;

private:
    NONCOPYABLE(Model)

public:
	std::vector<Mesh*> mMeshes;
	std::vector<MeshBuffer*> mMeshBuffers;
    std::vector<Texture*> mTextures;
};

#endif