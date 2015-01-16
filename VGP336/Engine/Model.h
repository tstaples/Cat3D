#ifndef INCLUDED_ENGINE_MODEL_H
#define INCLUDED_ENGINE_MODEL_H

#include "Common.h"

class Mesh;
class MeshBuffer;

class Model
{
public:
    Model() {}
    ~Model() {}
    void Destroy();

	std::vector<Mesh*> mMeshes;
	std::vector<MeshBuffer*> mMeshBuffers;
};

#endif