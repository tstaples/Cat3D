/* + Model
*  | + *Mesh
*  | | + Vertex buffer
*  | | + Index buffer
*  | | + Bone weights (max ~4 per vert)
*  | + *Bone
*  | + *Animations
*  | | + *Keyframes 
*/

// TODO: Unit test to import model then write it out, then repeat on the output file.
// Then diff the two output files to ensure they match. Ensures all data is recovered

#include "Importer.h"


Importer::Importer()
{
}

bool Importer::Load(const char* inputFile, f32 scale, u32 flags)
{
    // Load the model into the scene
	const aiScene* scene = mImporter.ReadFile(inputFile, flags);   
    ASSERT(scene, "Failed to load model - %s", mImporter.GetErrorString()); // TODO: Exception

    // Check for meshes and load them
    if (scene->HasMeshes())
    {
        LoadMeshes(*scene, scale);
    }

    if (scene->HasMaterials())
    {
        LoadMaterials(*scene);
    }

    if (scene->HasAnimations())
    {
        LoadAnimations(*scene);
    }
    return true;
}

//----------------------------------------------------------------------------------------------------

void Importer::Cleanup()
{
    for (auto mesh : mMeshes)
    {
        mesh->Destroy();
        delete mesh;
    }
    mMeshes.clear();
    
    for (auto bone : mBones)
    {
        delete bone;
    }
    mBones.clear();
    mBoneIndexMap.clear();
}

//----------------------------------------------------------------------------------------------------

void Importer::LoadMeshes(const aiScene& scene, f32 scale)
{
    const u32 numMeshes = scene.mNumMeshes;
    for (u32 i=0; i < numMeshes; ++i)
    {
        aiMesh* aimesh = scene.mMeshes[i];

        // Allocate space for the verts and indices
        const u32 numVerts = aimesh->mNumVertices;
        Mesh::Vertex* vertices = new Mesh::Vertex[numVerts];

        // 3 indices per face
        const u32 numIndices = (aimesh->mNumFaces * 3);
        u16* indices = new u16[numIndices];

        // Convert and copy the data from the imported file type to our native type
        CopyVertexData(*aimesh, scale, vertices);
        CopyIndexData(*aimesh, indices);

        // Store the native mesh
        Mesh* mesh = new Mesh();
        MeshBuilder::GenerateMesh(*mesh, vertices, numVerts, indices, numIndices);
        mMeshes.push_back(mesh);

        // Free temp
        SafeDeleteArray(vertices);
        SafeDeleteArray(indices);

        // Load bones for this mesh
        LoadBoneData(*aimesh, mesh);
    }
}

//----------------------------------------------------------------------------------------------------

void Importer::LoadBoneData(const aiMesh& aimesh, Mesh* mesh)
{
    if (aimesh.HasBones())
    {
        VertexWeights& vertexWeights = mesh->GetVertexWeights();
        vertexWeights.resize(aimesh.mNumVertices); // Each vertex has weights

        // Loop through all the bones for this mesh (not neccessarily all bones)
        const u32 numBones = aimesh.mNumBones;
        for (u32 i=0; i < numBones; ++i)
        {
            aiBone* aibone = aimesh.mBones[i];
            u32 boneIndex = 0;

            // see if we have already added this bone
            auto iter = mBoneIndexMap.find(aibone->mName.C_Str());
            if (iter != mBoneIndexMap.end())
            {
                boneIndex = iter->second;
            }
            else // This is a new bone
            {
                // Set index to the end
                boneIndex = mBones.size();
                Bone* newBone = new Bone();
                ASSERT(aibone->mName.length > 0, "[Importer] Bone %d doesn't have a name!", boneIndex);
                newBone->name = aibone->mName.C_Str();
                newBone->index = boneIndex;
                newBone->offsetTransform = *(Math::Matrix*)&aibone->mOffsetMatrix; // Hack: matrices have the same alignment

                // Add it to the array and remember it
                mBones.push_back(newBone);
                mBoneIndexMap.insert(std::make_pair(aibone->mName.C_Str(), boneIndex));
            }

            // Extract the bone weights (number of vertices this bone affects)
            const u32 numBoneWeights = aibone->mNumWeights;
            for (u32 j=0; j < numBoneWeights; ++j)
            {
                const aiVertexWeight& aivertexweight = aibone->mWeights[j];

                BoneWeight boneWeight;
                boneWeight.boneIndex = boneIndex;
                boneWeight.weight = aivertexweight.mWeight;

                // Store the weight for the corresponding vertex
                vertexWeights[aivertexweight.mVertexId].push_back(boneWeight);
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------

void Importer::LoadAnimations(const aiScene& scene)
{
    mpRoot = BuildSkeleton(*scene.mRootNode, nullptr);

    const u32 numAnimations = scene.mNumAnimations;
    for (u32 animIndex=0; animIndex < numAnimations; ++animIndex)
    {
        aiAnimation* aiAnimation = scene.mAnimations[animIndex];
        AnimationClip* animClip = new AnimationClip();

        ASSERT(aiAnimation->mName.length > 0, "[Importer] Animation %d doesn't have a name!", animIndex);
        animClip->mName = aiAnimation->mName.C_Str();
        animClip->mDuration = (f32)aiAnimation->mDuration;
        animClip->mTicksPerSecond = (f32)aiAnimation->mTicksPerSecond;

        if (animClip->mTicksPerSecond == 0.0f)
        {
            // If not specified, play back as 1:1 (second:tick)
            animClip->mTicksPerSecond = 1.0f;
        }

        const u32 numAnimChannels = aiAnimation->mNumChannels;
        for (u32 boneAnimIndex=0; boneAnimIndex < numAnimChannels; ++boneAnimIndex)
        {
            aiNodeAnim* aiNodeAnim = aiAnimation->mChannels[boneAnimIndex];
            BoneAnimation* boneAnim = new BoneAnimation();

            // Index of the bone this animation is for
            boneAnim->mBoneIndex = mBoneIndexMap.at(aiNodeAnim->mNodeName.C_Str());

            // Since we are storing entire transforms per keyframe rather than keyframing
            // position, rot and scale separately, ensure all keyframes match.
            // TODO: Support keyframes for the 3 sub channels
            ASSERT(aiNodeAnim->mNumPositionKeys == aiNodeAnim->mNumRotationKeys, "Invalid keyframe");
            ASSERT(aiNodeAnim->mNumPositionKeys == aiNodeAnim->mNumScalingKeys, "Invalid keyframe");

            // Assumes position keyframe index aligns with rotation and scaling keyframes
            for (u32 keyIndex=0; keyIndex < aiNodeAnim->mNumPositionKeys; ++keyIndex)
            {
                const aiVectorKey& aiPosKey     = aiNodeAnim->mPositionKeys[keyIndex];
                const aiQuatKey& aiRotKey       = aiNodeAnim->mRotationKeys[keyIndex];
                const aiVectorKey& aiScaleKey   = aiNodeAnim->mScalingKeys[keyIndex];

                // Ensure each keyframe is aligned on the timeline
                ASSERT(aiPosKey.mTime == aiRotKey.mTime, "Mismatched key time");
                ASSERT(aiPosKey.mTime == aiScaleKey.mTime, "Mismatched key time");

                Keyframe* keyframe = new Keyframe();
                keyframe->mTranslation  = ToV3(aiPosKey.mValue);
                keyframe->mRotation     = ToQ(aiRotKey.mValue);
                keyframe->mScale        = ToV3(aiScaleKey.mValue);
                keyframe->mTime         = (f32)aiPosKey.mTime;

                // Store the keyframe for this bone animation
                boneAnim->mKeyframes.push_back(keyframe);
            }

            // Store the bone animation for this animation clip
            animClip->mBoneAnimations.push_back(boneAnim);
        }
        // Store this animation clip
        mAnimations.push_back(animClip);
    }
}

//----------------------------------------------------------------------------------------------------

void Importer::LoadMaterials(const aiScene& scene)
{
    // Get the number of materials in the scene and iterate over them
    const u32 numMaterials = scene.mNumMaterials;
    for (u32 i=0; i < numMaterials; ++i)
    {
        // Get the material for this index
        aiMaterial* material = scene.mMaterials[i];

        // Get the number of diffuse textures in this material
        const u32 textureCount = material->GetTextureCount(aiTextureType_DIFFUSE);
        for (u32 j=0; j < textureCount; ++j)
        {
            // TODO: Get textures of all types

            // Get the path of each texture and store it.
            aiString texturePath;
            if (material->GetTexture(aiTextureType_DIFFUSE, j, &texturePath) == AI_SUCCESS)
            {
                mTexturePaths.push_back(texturePath.C_Str());
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------

void Importer::CopyVertexData(const aiMesh& aimesh, f32 scale, Mesh::Vertex* vertices)
{
    bool hasNormal = aimesh.HasNormals();
    bool hasTangents = aimesh.HasTangentsAndBitangents();
    
    const Math::Vector3 zero = Math::Vector3::Zero();

    const u32 numVertices = aimesh.mNumVertices;
    for (u32 j=0; j < numVertices; ++j)
    {
        // Cache the vertex
        Mesh::Vertex& vert = vertices[j];
    
        // Check if this vertex has colors
        bool hasColor = aimesh.HasVertexColors(j);

        // Assign corresponding data from importer, or default if doesn't exist
        vert.position   = ToV3(aimesh.mVertices[j]) * scale; // Scale position by param value
        vert.normal     = (hasNormal)   ? ToV3(aimesh.mNormals[j])      : zero;
        vert.tangent    = (hasTangents) ? ToV3(aimesh.mTangents[j])     : zero;
        vert.color      = (hasColor)    ? ToColor(*aimesh.mColors[j])   : Color::White();
    }

    // Check if this vertex has texture coords
    if (aimesh.HasTextureCoords(0))
    {
        // Get the number of UV channels
        const u32 numUVChannels = aimesh.GetNumUVChannels();
        for (u32 i=0; i < numUVChannels; ++i)
        {
            for (u32 j=0; j < numVertices; ++j)
            {
                const aiVector3D& tx = aimesh.mTextureCoords[i][j];
                vertices[j].texcoord = Math::Vector2(tx.x, tx.y);
            }
            break; // Hack: Our verts only support 1 channel
        }
    }
}

//----------------------------------------------------------------------------------------------------

void Importer::CopyIndexData(const aiMesh& aimesh, u16* indices)
{
    // Nothing to do here
    if (!aimesh.HasFaces())
        return;

    // Create local pointers for iterating
    const aiFace* aifaceIter = aimesh.mFaces;
    u16* indexIter = indices;

    const u32 numFaces = aimesh.mNumFaces;
    for (u32 j=0; j < numFaces; ++j)
    {
        // Get all 3 indices per face
        indexIter[0] = aifaceIter->mIndices[0];
	    indexIter[1] = aifaceIter->mIndices[1];
	    indexIter[2] = aifaceIter->mIndices[2];
        indexIter += 3;
        ++aifaceIter;
    }
}

//----------------------------------------------------------------------------------------------------

Bone* Importer::BuildSkeleton(aiNode& ainode, Bone* parent)
{
    Bone* bone = nullptr;

    // See if this bone already exists
    auto iter = mBoneIndexMap.find(ainode.mName.C_Str());
    if (iter == mBoneIndexMap.end())
    {
        // Create a new bone
        bone = new Bone();
        bone->index = mBones.size(); // append

        // Hack: If the bone has no name, assign temporary name
        if (ainode.mName.length > 0)
        {
            bone->name = ainode.mName.C_Str();
        }
        else
        {
            // TODO: use a GUID as temp name since this could be taken
            bone->name = "Unkown" + S(bone->index);
        }

        // Add the new bone to our array of bones
        mBones.push_back(bone);
        mBoneIndexMap.insert(std::make_pair(bone->name, bone->index));
    }
    else
    {
        // This bone already exists; added when we populated the meshes
        bone = mBones[iter->second];
    }

    // TODO: Set offset transform?
    bone->transform = *(Math::Matrix*)&ainode.mTransformation;
    bone->parent = parent;
    bone->parentIndex = (parent) ? parent->index : NO_PARENT;

    // Recursively populate children
    const u32 numChildren = ainode.mNumChildren;
    for (u32 i=0; i < numChildren; ++i)
    {
        // Build the child and add it to our list
        aiNode* aichild = ainode.mChildren[i];

        Bone* child = BuildSkeleton(*aichild , bone);
        bone->children.push_back(child);
        bone->childrenIndices.push_back(child->index); // Used for loading
    }
    return bone;
}

//----------------------------------------------------------------------------------------------------

Math::Vector3 Importer::ToV3(const aiVector3D& v)
{
    return Math::Vector3(v.x, v.y, v.z);
}

//----------------------------------------------------------------------------------------------------

Math::Quaternion Importer::ToQ(const aiQuaternion& q)
{
    return Math::Quaternion(q.x, q.y, q.z, q.w);
}

//----------------------------------------------------------------------------------------------------

Color Importer::ToColor(const aiColor4D& c)
{
    return Color(c.r, c.g, c.b, c.a);
}