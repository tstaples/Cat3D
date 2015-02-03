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

#include "Bone.h"

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

bool AssetLoader::LoadModel(const wchar_t* pFilename, Model& model)
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

bool AssetLoader::LoadCatmFile(const wchar_t* pFilename, Model& model)
{
    // Read the data into the buffer
    std::string filename = IO::WCharToChar(pFilename);
    FileBuffer buffer(filename.c_str());
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

        //LoadBones(sin, model);
    }
    return false;
}

//----------------------------------------------------------------------------------------------------

void AssetLoader::LoadMeshes(SerialReader& reader, Model& model)
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
}

//----------------------------------------------------------------------------------------------------

void AssetLoader::LoadTextures(const StringVec& paths, Model& model)
{
    const u32 numTextures = paths.size();
    for(auto path : paths)
    {
        wchar_t wbuffer[256];
        IO::CharToWChar(path, wbuffer, 256);

        Texture* texture = mTextureManager.GetResource(wbuffer);
        model.mTextures.push_back(texture);
    }
}

//----------------------------------------------------------------------------------------------------

void AssetLoader::LoadTexturesPaths(const wchar_t* pModelPath, SerialReader& reader, StringVec& paths)
{
    // Get the directory the model is located in (assumes textures are located in same directory)
    std::string modelpath = IO::WCharToChar(pModelPath); // HACK
    std::string modelDir = IO::GetLocation(modelpath);
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
}

void AssetLoader::LoadBones(SerialReader& reader, Model& model)
{
    const u32 numBones = reader.Read<u32>();
    model.mBones.resize(numBones);

    for (u32 i=0; i < numBones; ++i)
    {
        Bone* bone = new Bone();

        // Read the name
        const u32 nameLen = reader.Read<u32>();
        char nameBuffer[2048];
        reader.ReadArray(nameBuffer, nameLen);
        bone->name = nameBuffer;

        // Add the bone to the index map
        model.mBoneIndexMap.insert(std::make_pair(bone->name, i));

        bone->parentIndex = reader.Read<u32>();

        // Read in the children indices
        const u32 numChildIndices = reader.Read<u32>();
        reader.ReadVector(bone->childrenIndices, numChildIndices);

        // Read in the transform and offset transform
        bone->transform = reader.Read<Math::Matrix>();
        bone->offsetTransform = reader.Read<Math::Matrix>();
    }
}

void AssetLoader::LoadBoneWeights(SerialReader& reader, Model& model)
{
    const u32 numBoneWeights = reader.Read<u32>();
    //Mesh* mesh = model.mMeshes[i];
    //VertexWeights& vertexWeights = mesh->GetVertexWeights();
    //vertexWeights.resize(numBoneWeights);

    for (u32 i=0; i < numBoneWeights; ++i)
    {
        
    }
}