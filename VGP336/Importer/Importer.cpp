#include "Importer.h"


Importer::Importer()
{
    Init();
}

bool Importer::Init()
{
    // TODO: Load flags from command line
    mFlags =  aiProcess_Triangulate
			| aiProcess_JoinIdenticalVertices
			| aiProcess_SortByPType
			| aiProcess_FlipUVs;

    return true;
}

bool Importer::Load(const char* inputFile, f32 scale)
{
    // Load the model into the scene
	const aiScene* scene = mImporter.ReadFile(inputFile, mFlags);   
    ASSERT(scene, "Failed to load model - %s", mImporter.GetErrorString()); // TODO: Exception

    if (scene->HasMeshes())
    {
        LoadMeshes(*scene, scale);
    }

    if (scene->HasMaterials())
    {
        LoadMaterials(*scene);
    }
    return true;
}

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
    }
}

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

void Importer::CopyIndexData(const aiMesh& aimesh, u16* indices)
{
    if (!aimesh.HasFaces())
        return;

    const aiFace* aifaceIter = aimesh.mFaces;
    u16* indexIter = indices;

    const u32 numFaces = aimesh.mNumFaces;
    for (u32 j=0; j < numFaces; ++j)
    {
        indexIter[0] = aifaceIter->mIndices[0];
	    indexIter[1] = aifaceIter->mIndices[1];
	    indexIter[2] = aifaceIter->mIndices[2];
        indexIter += 3;
        ++aifaceIter;
    }
}

Math::Vector3 Importer::ToV3(const aiVector3D& v)
{
    return Math::Vector3(v.x, v.y, v.z);
}

Color Importer::ToColor(const aiColor4D& c)
{
    return Color(c.r, c.g, c.b, c.a);
}