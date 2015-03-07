#include "Precompiled.h"
#include "AssetLoader.h"

#include "AnimationClip.h"
#include "BoneAnimation.h"
#include "GraphicsSystem.h"
#include "Model.h"
#include "Mesh.h"
#include "MeshBuffer.h"
#include "MeshBuilder.h"
#include "TextureManager.h"
#include "Texture.h"
#include "IO.h"
#include "Path.h"
#include "FileBuffer.h"
#include "SerialReader.h"
#include "SerialWriter.h"

#include "Bone.h"

struct Header
{
    u32 signature;
    u32 version;

    Header() {}
    Header(u8 majver, u8 minver, const char* sig)
    {
        version = ((majver << 16) | ('.' << 8) | minver);
        signature = ( (sig[3] << 24) 
                    | (sig[2] << 16) 
                    | (sig[1] << 8) 
                    | (sig[0]));
    }

    inline std::pair<u8, u8> GetVersion() const
    {
        return GetVersion(version);
    }

    inline static std::pair<u8, u8> GetVersion(u32 v)
    {
        u8 maj = (v >> 16);
        u8 min = (v >> 24);
        return std::make_pair(maj, min);
    }
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
        const u32 version = sin.Read<u32>();

        // Load the mesh data into the model
        LoadMeshes(sin, model);

        // Load the texture paths from the file
        StringVec paths;
        LoadTexturesPaths(pFilename, sin, paths);

        // Load the textures from the files into the model
        LoadTextures(paths, model);

        // Load bone data and weights, then link up pointers
        LoadBones(sin, model);
        LoadBoneWeights(sin, model);
        LinkBones(model);

        // TODO: fix version
        //if (Header::GetVersion(version).second == 1)
        //{
        LoadAnimations(sin, model);
        //}
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
		meshBuffer->Initialize(*mpGraphicsSystem, Mesh::GetVertexFormat(), *mesh, true);
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

//----------------------------------------------------------------------------------------------------

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
        nameBuffer[nameLen] = '\0';
        bone->name = nameBuffer;

        // Add the bone to the index map
        model.mBoneIndexMap.insert(std::make_pair(bone->name, i));

        bone->parentIndex = reader.Read<u32>();
        bone->index = i;

        // Read in the children indices
        const u32 numChildIndices = reader.Read<u32>();
        reader.ReadVector(bone->childrenIndices, numChildIndices);

        // Read in the transform and offset transform
        bone->transform = reader.Read<Math::Matrix>();
        bone->offsetTransform = reader.Read<Math::Matrix>();

        // Store the bone in the model
        model.mBones[i] = bone;
    }
}

//----------------------------------------------------------------------------------------------------

void AssetLoader::LoadBoneWeights(SerialReader& reader, Model& model)
{
    for (auto mesh : model.mMeshes)
    {
        const u32 numBoneWeights = reader.Read<u32>();
        
        // Get the 2D vert weight array from each mesh and resize it
        VertexWeights& vertexWeights = mesh->GetVertexWeights();
        vertexWeights.resize(numBoneWeights);

        for (u32 i=0; i < numBoneWeights; ++i)
        {
            // Get how many bone weights this vert has
            const u32 numWeightsForThisVert = reader.Read<u32>();
            for (u32 j=0; j < numWeightsForThisVert; ++j)
            {
                // Store the boneweight at the corresponding index for this vert
                BoneWeight boneWeight = reader.Read<BoneWeight>();
                vertexWeights[i].push_back(boneWeight);
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------

void AssetLoader::LinkBones(Model& model)
{
    std::vector<Bone*>& bones = model.mBones;
    for (auto bone : bones)
    {
        // Check the bone has a parent
        const u32 parentIndex = bone->parentIndex;
        if (parentIndex != NO_PARENT)
        {
            // Link parent pointer to element at parentIndex
            bone->parent = bones[parentIndex];
        }
        else
        {
            // No parent; this bone is the root
            model.mpRoot = bone;
        }

        // Expand children array to number of children
        const u32 numChildren = bone->childrenIndices.size();
        bone->children.resize(numChildren);

        for (u32 i=0; i < numChildren; ++i)
        {
            // Store pointer to bone at childIndex's position in bone array
            const u32 childIndex = bone->childrenIndices[i];
            bone->children[i] = bones[childIndex];
        }
    }
}

//----------------------------------------------------------------------------------------------------

/*
 *  + Animations:
 *  | + Num animation clips     (4)
 *  | + Animation Clip
 *  | | + Length encoded name   (4 + c)
 *  | | + Duration              (4)
 *  | | + TicksPerSecond        (4)
 *  | | + KeyFrameCount         (4)
 *  | | + Num bone animations   (4)
 *  | | + BoneAnimation
 *  | | | + Bone index          (4)
 *  | | | + Num keyframes       (4)
 *  | | | + Keyframes
 *  | | | | + Translation       (12)
 *  | | | | + Rotation          (16)
 *  | | | | + Scale             (12)
 *  | | | | + Time              (4)
*/

void AssetLoader::LoadAnimations(SerialReader& reader, Model& model)
{
    // Get the number of animation clips
    const u32 numAnimClips = reader.Read<u32>();
    model.mAnimations.resize(numAnimClips);

    for (u32 i=0; i < numAnimClips; ++i)
    {
        // Read in the clip data
        AnimationClip* animClip = new AnimationClip();
        animClip->mName              = reader.ReadLengthEncodedString();
        animClip->mDuration          = reader.Read<f32>();
        animClip->mTicksPerSecond    = reader.Read<f32>();

        // Get the number of bone animations and resize the array
        const u32 numBoneAnimations = reader.Read<u32>();
        animClip->mBoneAnimations.resize(numBoneAnimations);
        for (u32 j=0; j < numBoneAnimations; ++j)
        {
            BoneAnimation* boneAnim = new BoneAnimation();
            boneAnim->mBoneIndex = reader.Read<u32>();

            // Get the number of keyframes and resize the array
            const u32 numKeyframes = reader.Read<u32>();
            boneAnim->mKeyframes.resize(numKeyframes);
            for (u32 k=0; k < numKeyframes; ++k)
            {
                // Read in the keyframe
                boneAnim->mKeyframes[k] = new Keyframe();
                *boneAnim->mKeyframes[k] = reader.Read<Keyframe>();
            }
            animClip->mBoneAnimations[j] = boneAnim;
        }
        model.mAnimations[i] = animClip;
    }
}