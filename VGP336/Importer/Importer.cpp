#include "Importer.h"


Importer::Importer()
{
    Init();
}

bool Importer::Init()
{
    // TODO: Load flags from config
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

    if (!scene->HasMeshes())
    {
        // TODO: decide how to handle empty models
        return false;
    }

    const u32 numMeshes = scene->mNumMeshes;
    for (u32 i=0; i < numMeshes; ++i)
    {
        aiMesh* aimesh = scene->mMeshes[i];

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
    return true;
}

void Importer::CopyVertexData(const aiMesh& aimesh, f32 scale, Mesh::Vertex* vertices)
{
    bool hasNormal = aimesh.HasNormals();
    bool hasTangents = aimesh.HasTangentsAndBitangents();
    
    const Math::Vector3 zero = Math::Vector3::Zero();

    const u32 numVertices = aimesh.mNumVertices;
    for (u32 j=0; j < numVertices; ++j)
    {
        Mesh::Vertex& vert = vertices[j];
    
        bool hasColor = aimesh.HasVertexColors(j);
        bool hasUV = aimesh.HasTextureCoords(j);

        vert.position   = ToV3(aimesh.mVertices[j]) * scale; // Scale position by set value
        vert.normal     = (hasNormal)   ? ToV3(aimesh.mNormals[j])      : zero;
        vert.tangent    = (hasTangents) ? ToV3(aimesh.mTangents[j])     : zero;
        vert.color      = (hasColor)    ? ToColor(*aimesh.mColors[j])   : Color::White();
        // TODO: UV coords & handling multple
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