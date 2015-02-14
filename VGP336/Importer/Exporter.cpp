#include "Exporter.h"

#include <fstream>
#include "FileBuffer.h"
#include "IO.h"

#define VERSION_MAJOR 1
#define VERSION_MINOR 1

/* === Format Guidelines ===
 * Layout:
 *	+ Header                    (8)
 *	+ Number of meshes          (4)
 *	+ Vertex block:             [64]
 *	|	Number of verts         (4)
 *	|	Position                (12)
 *	|	Normal                  (12)
 *	|	Tangent                 (12)
 *	|	Color                   (16)
 *	|	Texture Coords          (8)
 *	+ Indices block:            [4 + 2n]
 *	|	Number of indicies      (4)
 *  |   indices...              (2 bytes each)
 *  + Texture Paths:            [n(4 + c) + 4]
 *  |   Number of paths         (4)
 *  |   Length byte + Path      (4 + c)
 *  + Bones:                    [16 + 
 *  |   Num bones               (4)
 *  |   Name length             (4)
 *  |   Name                    (strlen)
 *  |   Parent index            (4)
 *  |   Num child indices       (4)
 *  |   Child indices           (4 each)
 *  |   transform               (16 * 4)
 *  |   offset transform        (16 * 4)
 *  + Bone Weights:
 *  |    Num bone weights       (4)
 *  |    Num weights for this vert (numVerts * 4)
 *  |    Weights... (4 * Num weights for this vert)
 *
 *  VERSION 1.1
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

//  TODO:
//  skeleton -> bone array
//      - name (useful during runtime for referring to specific bone)
//      - parent index
//      - children index
//      - transform
//      - offset transform
//  Index does not need to be saved since they're saved sequencially
//
//  **When loading, load all  the bones first, then do a second pass to connect parents and children using the indices written to the file
// 
//  Vertex bone weights
//      - number of weights for that vertex
//      - weights...


bool Exporter::Export(const char* outpath, 
                      const Meshes& meshes, 
                      const StringVec& texPaths, 
                      const BoneVec& bones, 
                      const Animations& animations)
{
    // Get total size of all mesh data
    size_t size = CalculateSize(meshes, texPaths, bones, animations);

    // Init our buffer
    FileBuffer buffer(size);

    // Write header (TODO: read version from config)
    Header header(VERSION_MAJOR, VERSION_MINOR, "CATM");
    buffer.Write(header);

    // Export all the model data
    ExportMeshes(meshes, buffer);
    ExportTextures(texPaths, buffer);
    ExportBones(bones, buffer);
    ExportBoneWeights(meshes, buffer);

#if VERSION_MINOR == 1
    ExportAnimations(animations, buffer);
#endif

    // Output the buffer
    bool rc = IO::SyncWriteFile(outpath, buffer.GetBuffer(), size);
    buffer.Clear();

    return rc;
}

//----------------------------------------------------------------------------------------------------

void Exporter::ExportMeshes(const Meshes& meshes, FileBuffer& buffer)
{
    // Write how many meshes this file contains
    const u32 numMeshes = meshes.size();
    buffer.Write(numMeshes);

    // Write all the vertex and index data
    for (auto &mesh : meshes)
    {
        // Write the number of verts then all the verts
        const u32 numVerts = mesh->GetVertexCount();
        buffer.Write(numVerts);
        buffer.WriteArray(mesh->GetVertices(), numVerts * sizeof(Mesh::Vertex));

        // Write the number of indices and index buffer
        const u32 numIndices = mesh->GetIndexCount();
        buffer.Write(numIndices);
        buffer.WriteArray(mesh->GetIndices(), numIndices * sizeof(u16));
    }
}

//----------------------------------------------------------------------------------------------------

void Exporter::ExportTextures(const StringVec& texPaths, FileBuffer& buffer)
{
    // Write the number of textures
    const u32 numTextures = texPaths.size();
    buffer.Write(numTextures);

    for (auto& path : texPaths)
    {
        // Length encode each string
        u32 pathSize = path.length();
        buffer.Write(pathSize);

        // Write it to the buffer
        buffer.WriteArray(path.c_str(), pathSize);
    }
}

//----------------------------------------------------------------------------------------------------

void Exporter::ExportBones(const BoneVec& bones, FileBuffer& buffer)
{
    // Write how many bones there are
    const u32 numBones = bones.size();
    buffer.Write(numBones);

    for (auto bone : bones)
    {
        // Write length encoded string
        u32 nameLen = bone->name.size();
        buffer.Write(nameLen);
        buffer.WriteArray(bone->name.c_str(), nameLen);

        // Write the parent index
        buffer.Write(bone->parentIndex);

        // Write number of child indices, then the indices themselves
        const u32 numChildrenIndices = bone->childrenIndices.size();
        buffer.Write(numChildrenIndices);
        for (u32 i=0; i < numChildrenIndices; ++i)
        {
            u32 childIndex = bone->childrenIndices[i];
            buffer.Write(childIndex);
        }
        //buffer.WriteVector(bone->childrenIndices);

        // Write transform and offset transform matrices
        buffer.Write(bone->transform);
        buffer.Write(bone->offsetTransform);
    }
}

//----------------------------------------------------------------------------------------------------

void Exporter::ExportBoneWeights(const Meshes& meshes, FileBuffer& buffer)
{
    for (auto mesh : meshes)
    {
        VertexWeights& vertexWeights = mesh->GetVertexWeights();

        // Write the number of weights
        const u32 numBoneWeights = vertexWeights.size();
        buffer.Write(numBoneWeights);

        // Iterate through all the weights for each vertex
        for (auto weights : vertexWeights)
        {
            // Write how many weights this vertex contains, then the weights
            const u32 numWeightsForThisVert = weights.size();
            buffer.Write(numWeightsForThisVert);
            for (auto weight : weights)
            {
                buffer.Write(weight);
            }
            //buffer.WriteVector(weights);
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

// NOTE: When loading, resize model.mBoneAnimations to number of bones and init each element to null
void Exporter::ExportAnimations(const Animations& animations, FileBuffer& buffer)
{
    // Write number of animation clips
    const u32 numAnimations = animations.size();
    buffer.Write(numAnimations);

    for (auto animClip : animations)
    {
        // Animation clip data
        WriteLengthEncodedString(animClip->mName, buffer);
        buffer.Write(animClip->mDuration);
        buffer.Write(animClip->mTicksPerSecond);
        
        // Bone animations data
        const u32 numBoneAnims = animClip->mBoneAnimations.size();
        buffer.Write(numBoneAnims);
        for (auto boneAnim : animClip->mBoneAnimations)
        {
            // Index of the bone this animation corresponds to
            buffer.Write(boneAnim->mBoneIndex);

            // Write all the keyframe data
            const u32 numKeyframes = boneAnim->mKeyframes.size();
            buffer.Write(numKeyframes);
            for (Keyframe* keyframe : boneAnim->mKeyframes)
            {
                buffer.Write(*keyframe);
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------


size_t Exporter::CalculateSize(const Meshes& meshes, const StringVec& texPaths, const BoneVec& bones, const Animations& animations)
{
    size_t size = 0;

    size += sizeof(Header);
    size += sizeof(u32);                 // Number of Meshes
    size += sizeof(u32) * meshes.size(); // Number of verticies
    size += sizeof(u32) * meshes.size(); // Number of indices

    for (auto& mesh : meshes)
    {
        // Vertex and index blocks
        size += mesh->GetVertexCount() * sizeof(Mesh::Vertex);
        size += mesh->GetIndexCount() * sizeof(u16);

        // Size of all the weights per mesh
        size += sizeof(u32); // number of bone weights
        for (auto weights : mesh->GetVertexWeights())
        {
            size += sizeof(u32); // number of bone weights for this vert
            size += weights.size() * sizeof(BoneWeight);
        }
    }

    // Iterate through all the paths and get their size
    for (auto& path : texPaths)
    {
        size += sizeof(u32);    // Length encoding byte
        size += path.length();
    }
    size += sizeof(u32); // Number of textures

    // === Bones ===
    size += sizeof(u32); // Number of bones   
    for (auto bone : bones)
    {
        size += sizeof(u32); // length encode byte
        size += bone->name.length();
        size += sizeof(u32); // parent index
        size += sizeof(u32); // number of child indices
        size += bone->childrenIndices.size() * sizeof(u32);
        size += (sizeof(Math::Matrix) * 2); // transform and offset transform
    }

#if VERSION_MINOR == 1
    // === Animations ===
    size += sizeof(u32); // Number of animation clips
    for (auto animClip : animations)
    {
        size += sizeof(u32);    // length encode
        size += animClip->mName.length();
        size += sizeof(f32);    // duration
        size += sizeof(f32);    // ticks per second
        size += sizeof(u32);    // number of bone animations
        for (auto boneAnim : animClip->mBoneAnimations)
        {
            size += sizeof(u32);    // Bone index
            size += sizeof(u32);    // Num keyframes
            size += sizeof(Keyframe) * boneAnim->mKeyframes.size();
        }
    }
#endif
    return size;
}

//----------------------------------------------------------------------------------------------------

void Exporter::WriteLengthEncodedString(const std::string& str, FileBuffer& buffer)
{
    u32 len = str.length();
    buffer.Write(len);
    buffer.WriteArray(str.c_str(), len);
}