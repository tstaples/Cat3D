#include "Precompiled.h"
#include "AssetLoader.h"
#include "Model.h"
#include "Mesh.h"
#include "MeshBuffer.h"
#include "MeshBuilder.h"
#include "File.h"
#include "FileBuffer.h"
#include "Serializer.h"
#include "GraphicsSystem.h"

struct Header
{
    u32 signature;
    u32 version;
};

bool AssetLoader::LoadModel(const char* pFilename, GraphicsSystem& gs, Model& model)
{
    // Get the asset type and pass to corresponding function
    std::string extension = IO::File::GetExtension(pFilename);
    if (extension.compare("catm") == 0)
    {
        return LoadCatmFile(pFilename, gs, model);
    }
    return false;
}

bool AssetLoader::LoadCatmFile(const char* pFilename, GraphicsSystem& gs, Model& model)
{
    // Read the data into the buffer
    FileBuffer buffer(pFilename);
    if (buffer.Initialized())
    {
        // Init the reader with the model data
        SerialReader sin(buffer.GetBuffer(), buffer.Size());

        // Read the signature and check that it matches
        std::string sig = sin.ReadFormatted(4);
        if (sig.compare("CATM") != 0)
        {
            return false;
        }
        
        // TODO: eventually do something with version number.
        // Skip past it for now
        sin.Seek(sizeof(u32), SerialReader::Current);

        u32 numMeshes = sin.Read<u32>();
        for (u32 i=0; i < numMeshes; ++i)
        {
            // Read the verticies
            u32 numVerts = sin.Read<u32>();
            Mesh::Vertex* vertexBuffer = new Mesh::Vertex[numVerts];
            sin.Read(*vertexBuffer, numVerts);
            //sin.Read(*vertexBuffer);
            //sin.Seek((numVerts-1) * sizeof(Mesh::Vertex), SerialReader::Current); // hack

            // Read in the indices
            u32 numIndices = sin.Read<u32>();
            u16* indexBuffer = new u16[numIndices];
            sin.Read(*indexBuffer, numIndices);

            Mesh* mesh = new Mesh();
            MeshBuilder::GenerateMesh(*mesh, vertexBuffer, numVerts, indexBuffer, numIndices * 3);
			model.mMeshes.push_back(mesh);

			SafeDeleteArray(vertexBuffer);
			SafeDeleteArray(indexBuffer);

			MeshBuffer* meshBuffer = new MeshBuffer();
			meshBuffer->Initialize(gs, Mesh::GetVertexFormat(), *mesh);
			model.mMeshBuffers.push_back(meshBuffer);
        }
    }
    return false;
}