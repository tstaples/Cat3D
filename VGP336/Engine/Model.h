#ifndef INCLUDED_ENGINE_MODEL_H
#define INCLUDED_ENGINE_MODEL_H

#include "Common.h"

namespace Math
{
    struct Matrix;
}
struct Bone;
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
// TODO: Friend assetloader and make these private
	std::vector<Mesh*> mMeshes;

    Bone* mpRoot; // weak ptr
    std::vector<Bone*> mBones;
    // Used to lookup bones by name O(logn) (offline)
    std::map<std::string, u32> mBoneIndexMap;

	std::vector<MeshBuffer*> mMeshBuffers;
    std::vector<Texture*> mTextures;
};

#endif