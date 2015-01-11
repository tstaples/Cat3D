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

    MeshPtr mesh(nullptr);
    const u32 numMeshes = scene->mNumMeshes;
    for (u32 i=0; i < numMeshes; ++i)
    {
        //AIMeshPtr aimesh(scene->mMeshes[i]);
        aiMesh* aimesh = scene->mMeshes[i];
        mesh.reset(new LocalMesh());

        // Allocate space for the verts and indices
        mesh->mVertices.resize(aimesh->mNumVertices);
        mesh->mIndices.resize(aimesh->mNumFaces * 3);

        // Convert and copy the data from the imported file type to our native type
        CopyVertexData(*aimesh, scale, mesh);
        CopyIndexData(*aimesh, mesh);

        // Store the native mesh
        mMeshes.push_back(std::move(mesh));
    }
    return true;
}

void Importer::CopyVertexData(const aiMesh& aimesh, f32 scale, MeshPtr& mesh)
{
    bool hasNormal = aimesh.HasNormals();
    bool hasTangents = aimesh.HasTangentsAndBitangents();
    
    const Math::Vector3 zero = Math::Vector3::Zero();

    const u32 numVertices = aimesh.mNumVertices;
    for (u32 j=0; j < numVertices; ++j)
    {
        Mesh::Vertex& vert = mesh->mVertices[j];
    
        bool hasColor = aimesh.HasVertexColors(j);
        bool hasUV = aimesh.HasTextureCoords(j);

        vert.position   = ToV3(aimesh.mVertices[j]); // always present
        vert.normal     = (hasNormal)   ? ToV3(aimesh.mNormals[j])      : zero;
        vert.tangent    = (hasTangents) ? ToV3(aimesh.mTangents[j])     : zero;
        vert.color      = (hasColor)    ? ToColor(*aimesh.mColors[j])   : Color::White();
        // TODO: UV coords & handling multple
    }
}

void Importer::CopyIndexData(const aiMesh& aimesh, MeshPtr& mesh)
{
    if (!aimesh.HasFaces())
        return;

    const aiFace* aifaceIter = aimesh.mFaces;
    IndexList::iterator indexIter = mesh->mIndices.begin();

    const u32 numFaces = aimesh.mNumFaces;
    for (u32 j=0; j < numFaces; ++j)
    {
        indexIter[0] = aifaceIter->mIndices[0];
	    indexIter[1] = aifaceIter->mIndices[1];
	    indexIter[2] = aifaceIter->mIndices[2];
        indexIter += 3;
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