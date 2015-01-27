#include "Precompiled.h"
#include "AssetLoader.h"

#include "GraphicsSystem.h"
#include "Model.h"
#include "Mesh.h"
#include "MeshBuffer.h"
#include "MeshBuilder.h"
#include "TextureManager.h"
#include "Texture.h"
#include "IO.h"
#include "FileBuffer.h"
#include "Serializer.h"
#include "Path.h"

struct Header
{
    u32 signature;
    u32 version;
};

AssetLoader::AssetLoader()
    : mpGraphicsSystem(nullptr)
{
}

//----------------------------------------------------------------------------------------------------

void AssetLoader::Initialize(GraphicsSystem& gs)
{
    mpGraphicsSystem = &gs;
    mTextureManager.Initialize(gs);
}

//----------------------------------------------------------------------------------------------------

void AssetLoader::Terminate()
{
    mTextureManager.Terminate();
    mpGraphicsSystem = nullptr;
}

//----------------------------------------------------------------------------------------------------

bool AssetLoader::LoadModel(const char* pFilename, Model& model)
{
    // Get the asset type and pass to corresponding function
    std::string extension = IO::GetExtension(pFilename);
    if (extension.compare("catm") == 0)
    {
        return LoadCatmFile(pFilename, model);
    }
    return false;
}

//----------------------------------------------------------------------------------------------------

bool AssetLoader::LoadCatmFile(const char* pFilename, Model& model)
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

        // Load the mesh data into the model
        LoadMeshes(sin, model);

        // Load the texture paths from the file
        StringVec paths;
        LoadTexturesPaths(pFilename, sin, paths);

        // Load the textures from the files into the model
        LoadTextures(paths, model);
    }
    return false;
}

//----------------------------------------------------------------------------------------------------

bool AssetLoader::LoadMeshes(SerialReader& reader, Model& model)
{
    u32 numMeshes = reader.Read<u32>();
    for (u32 i=0; i < numMeshes; ++i)
    {
        // Read the verticies
        u32 numVerts = reader.Read<u32>();
        Mesh::Vertex* vertexBuffer = new Mesh::Vertex[numVerts];
        reader.ReadArray(vertexBuffer, sizeof(Mesh::Vertex) * numVerts);

        // Read in the indices
        u32 numIndices = reader.Read<u32>();
        u16* indexBuffer = new u16[numIndices];
        reader.ReadArray(indexBuffer, sizeof(u16) * numIndices);

        // Generate the push and add it to the model
        Mesh* mesh = new Mesh();
        MeshBuilder::GenerateMesh(*mesh, vertexBuffer, numVerts, indexBuffer, numIndices);
		model.mMeshes.push_back(mesh);

        // Clear the temp buffers
		SafeDeleteArray(vertexBuffer);
		SafeDeleteArray(indexBuffer);

        // Create the mesh buffer and add it to the model
		MeshBuffer* meshBuffer = new MeshBuffer();
		meshBuffer->Initialize(*mpGraphicsSystem, Mesh::GetVertexFormat(), *mesh);
		model.mMeshBuffers.push_back(meshBuffer);
    }
    return true;
}

//----------------------------------------------------------------------------------------------------

bool AssetLoader::LoadTextures(const StringVec& paths, Model& model)
{
    const u32 numTextures = paths.size();
    for(auto path : paths)
    {
        wchar_t wbuffer[256];
        IO::CharToWChar(path, wbuffer, 256);

        Texture* texture = mTextureManager.GetResource(wbuffer);
        model.mTextures.push_back(texture);
    }
    return true;
}

//----------------------------------------------------------------------------------------------------

bool AssetLoader::LoadTexturesPaths(const char* pModelPath, SerialReader& reader, StringVec& paths)
{
    std::string modelDir = IO::GetLocation(pModelPath);
    char tempPathBuffer[MAX_PATH];

    u32 numTextures = reader.Read<u32>();
    for (u32 i=0; i < numTextures; ++i)
    {
        // Read the length encode byte
        u32 pathLength = reader.Read<u32>();

        // Read pathLength # of bytes into the temp array
        reader.ReadArray(tempPathBuffer, pathLength);
        tempPathBuffer[pathLength] = '\0'; // Terminate the string

        // Store the FULL path and clear the buffer
        paths.push_back(modelDir + tempPathBuffer);
        memset(tempPathBuffer, 0, MAX_PATH);
    }
    return true;
}