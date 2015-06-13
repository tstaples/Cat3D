#ifndef INCLUDED_ENGINE_TERRAIN_H
#define INCLUDED_ENGINE_TERRAIN_H

//====================================================================================================
// Filename:	Terrain.h
// Created by:	Peter Chan
// Description:	Class for rendering a terrain.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "ConstantBuffer.h"
#include "Heightmap.h"
#include "Mesh.h"
#include "MeshBuffer.h"
#include "Meta.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "Texture.h"
#include "VertexShader.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

namespace Math
{
	struct Matrix;
}
class Camera;
class GraphicsSystem;

//====================================================================================================
// Class Declarations
//====================================================================================================

class Terrain
{
public:

    struct Layer
    {
        META_DECLARE_CLASS

        u32 layerIndex;
        f32 minHeight;
        f32 maxHeight;
        f32 frequency;
        std::string texturePath;
    };

	Terrain();
	~Terrain();

    // width & length are dimensions of the heightmap file
	void Initialize(GraphicsSystem& gs, const char* pFilename, u32 width, u32 length, f32 maxHeight, const Math::Vector3& pos);
	void Terminate();

	void SetCamera(Camera& camera);
	void SetLayer(Texture* pTexture, u32 layer, f32 minHeight, f32 maxHeight, f32 frequency);
	void SetLayer(Texture* pTexture, const Layer& layer);

	f32 GetHeight(const Math::Vector3& position);

	void Render(const Math::Matrix& transform);

private:
	NONCOPYABLE(Terrain);

	static const u32 kMaxTerrainLayers = 4;

	struct CBuffer
	{
		XMMATRIX matWVP;
		XMVECTOR layer[kMaxTerrainLayers];
	};

	GraphicsSystem* mpGraphicsSystem;
	Camera* mpCamera;

	TypedConstantBuffer<CBuffer> mCBuffer;

	VertexShader mVertexShader;
	PixelShader mPixelShader;

	Heightmap mHeightmap;   // Raw 2d byte array
	Mesh mTerrainMesh;
	MeshBuffer mTerrain;

	Texture* mpTerrainLayers[kMaxTerrainLayers];
	Math::Vector3 mLayerInfo[kMaxTerrainLayers];

	Sampler mSampler;

	bool mInitialized;
};

#endif // #ifndef INCLUDED_ENGINE_TERRAIN_H