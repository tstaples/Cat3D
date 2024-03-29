//====================================================================================================
// Filename:	Terrain.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "Terrain.h"

#include "Camera.h"
#include "GraphicsSystem.h"
#include "MeshBuilder.h"
#include "MeshBuffer.h"
#include "Texture.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

META_CLASS_BEGIN(Terrain::Layer)
META_FIELD_BEGIN
    META_FIELD(layerIndex, "LayerIndex")
    META_FIELD(minHeight, "MinHeight")
    META_FIELD(maxHeight, "MaxHeight")
    META_FIELD(frequency, "Frequency")
    META_FIELD(texturePath, "TexturePath")
META_FIELD_END
META_CLASS_END

Terrain::Terrain()
	: mpGraphicsSystem(nullptr)
	, mpCamera(nullptr)
	, mInitialized(false)
{
	for (u32 i = 0; i < kMaxTerrainLayers; ++i)
	{
		mpTerrainLayers[i] = nullptr;
	}
}

//----------------------------------------------------------------------------------------------------

Terrain::~Terrain()
{
	ASSERT(!mInitialized, "[Terrain] Not terminated!");
}

//----------------------------------------------------------------------------------------------------

void Terrain::Initialize(GraphicsSystem& gs, const char* pFilename, u32 width, u32 length, f32 maxHeight, const Math::Vector3& pos)
{
	ASSERT(!mInitialized, "[Terrain] Already initialized!");

	mpGraphicsSystem = &gs;

	mCBuffer.Initialize(gs);

	const u32 vertexFormat = Mesh::GetVertexFormat();
	mVertexShader.Initialize(gs, L"../Engine/Shaders/Terrain.fx", "VS", "vs_4_0", vertexFormat);
	mPixelShader.Initialize(gs, L"../Engine/Shaders/Terrain.fx", "PS", "ps_4_0");

	mHeightmap.GenerateFromRAW(pFilename, width, length);
	MeshBuilder::CreateTerrain(mTerrainMesh, pos, mHeightmap, maxHeight);
	mTerrain.Initialize(gs, vertexFormat, mTerrainMesh);

	mSampler.Initialize(gs, Sampler::Anisotropic, Sampler::Wrap);

	mInitialized = true;
}

//----------------------------------------------------------------------------------------------------

void Terrain::Terminate()
{
	ASSERT(mInitialized, "[Terrain] Already terminated!");

	mpGraphicsSystem = nullptr;
	mpCamera = nullptr;

	mCBuffer.Terminate();

	mVertexShader.Terminate();
	mPixelShader.Terminate();

    mHeightmap.Purge();
	mTerrain.Terminate();
	mTerrainMesh.Destroy();

	for (u32 i = 0; i < kMaxTerrainLayers; ++i)
	{
		SafeRelease(mpTerrainLayers[i]);
	}

	mSampler.Terminate();

	mInitialized = false;
}

//----------------------------------------------------------------------------------------------------

void Terrain::SetCamera(Camera& camera)
{
	mpCamera = &camera;
}

//----------------------------------------------------------------------------------------------------

void  Terrain::SetLayer(Texture* pTexture, u32 layer, f32 minHeight, f32 maxHeight, f32 frequency)
{
	ASSERT(layer < kMaxTerrainLayers, "[Terrain] Only support %d layers.", kMaxTerrainLayers);
	ASSERT(minHeight <= maxHeight && frequency > 0.0f, "[Terrain] Invalid layer params.");

	mpTerrainLayers[layer] = pTexture;
	mLayerInfo[layer].x = minHeight;
	mLayerInfo[layer].y = maxHeight;
	mLayerInfo[layer].z = frequency;
}

//----------------------------------------------------------------------------------------------------

void Terrain::SetLayer(Texture* pTexture, const Layer& layer)
{
    SetLayer(pTexture, layer.layerIndex, layer.minHeight, layer.maxHeight, layer.frequency);
}

//----------------------------------------------------------------------------------------------------

f32 Terrain::GetHeight(const Math::Vector3& position)
{
	const u32 rows = mHeightmap.GetRows();
	const u32 cols = mHeightmap.GetColumns();
	const f32 length = (f32)rows - 1.0f;
	const f32 width = (f32)cols - 1.0f;
	if (position.x < 0.0f || position.x > width ||
		position.z < 0.0f || position.z > length)
	{
		return 0.0f;
	}

    // Truncate position to an index
	const u32 minX = (u32)position.x;
	const u32 minZ = (u32)position.z;
	const u32 maxX = minX + 1;
	const u32 maxZ = minZ + 1;

	//
	//  B---C
	//  | / |
	//	A---D
	//


    // Get the y value of the 4 verts in the quad
	const Mesh::Vertex* vertices = mTerrainMesh.GetVertices();
	const f32 a = vertices[minX + (minZ * cols)].position.y;
	const f32 b = vertices[minX + (maxZ * cols)].position.y;
	const f32 c = vertices[maxX + (maxZ * cols)].position.y;
	const f32 d = vertices[maxX + (minZ * cols)].position.y;

    // Find actual position (%) in the cell
	const f32 deltaX = position.x - (f32)minX;
	const f32 deltaZ = position.z - (f32)minZ;

	f32 height = a;
	if (deltaX > deltaZ) // Bottom half of the quad
	{
        // Start from d and lerp x and y the amount we're into the cell
		height = d + ((1.0f - deltaX) * (a - d)) + (deltaZ * (c - d));
	}
	else
	{
        // Start from b and lerp x and y the amount we're into the cell
		height = b + ((1.0f - deltaZ) * (a - b)) + (deltaX * (c - b));
	}
	return height;
}

//----------------------------------------------------------------------------------------------------

void Terrain::Render(const Math::Matrix& transform)
{
	ASSERT(mpCamera != nullptr , "[Terrain] No camera set!");
	ASSERT(mpTerrainLayers[0] != nullptr, "[Terrain] At least one layer must be set!");

	XMMATRIX matWorld = *(XMMATRIX*)&transform;
	XMMATRIX matView = *(XMMATRIX*)&mpCamera->GetViewMatrix();
	XMMATRIX matProj = *(XMMATRIX*)&mpCamera->GetProjectionMatrix();

	CBuffer cb;
	cb.matWVP = XMMatrixTranspose(matWorld * matView * matProj);
	for (u32 i = 0; i < kMaxTerrainLayers; ++i)
	{
		if (mpTerrainLayers[i] != nullptr)
		{
			cb.layer[i] = XMLoadFloat3((XMFLOAT3*)&mLayerInfo[i]);
			mpTerrainLayers[i]->BindPS(*mpGraphicsSystem, i);
		}
		else
		{
			cb.layer[i] = XMVectorSet(F32_MAX, F32_MAX, 0.0f, 0.0f);
		}
	}
	mCBuffer.Set(*mpGraphicsSystem, cb);
	mCBuffer.BindVS(*mpGraphicsSystem, 0);
	mCBuffer.BindPS(*mpGraphicsSystem, 0);

	mSampler.BindPS(*mpGraphicsSystem, 0);

	mVertexShader.Bind(*mpGraphicsSystem);
	mPixelShader.Bind(*mpGraphicsSystem);

	mTerrain.Render(*mpGraphicsSystem);
}