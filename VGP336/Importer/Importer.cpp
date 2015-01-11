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
    AIScenePtr scene(mImporter.ReadFile(inputFile, mFlags));
	// TODO: Exception
    ASSERT(scene, "Failed to load model - %s", mImporter.GetErrorString());

    if (!scene->HasMeshes())
    {
        // TODO: decide how to handle empty models
        return false;
    }

    const u32 numMeshes = scene->mNumMeshes;
    for (u32 i=0; i < numMeshes; ++i)
    {
        AIMeshPtr aimesh(scene->mMeshes[i]);
        MeshPtr mesh(new LocalMesh());

        // Allocate space for the verts and indices
        mesh->mVertices.resize(aimesh->mNumVertices);
        mesh->mIndices.resize(aimesh->mNumFaces * 3);

        // Convert and copy the data from the imported file type to our native type
        CopyVertexData(aimesh, scale, mesh);
        CopyIndexData(aimesh, mesh);

        // Store the native mesh
        mMeshes.push_back(std::move(mesh));
    }
    return true;
}

void Importer::CopyVertexData(const AIMeshPtr& aimesh, f32 scale, MeshPtr& mesh)
{
    bool hasNormal = aimesh->HasNormals();

    NativeVertList::iterator vIter = mesh->mVertices.begin();
    const aiVector3D* aivIter = aimesh->mVertices;
    const u32 numVertices = aimesh->mNumVertices;

    for (u32 j=0; j < numVertices; ++j)
    {
        CopyPosition(*aivIter, *vIter, scale);
        if (hasNormal)
        {
            // Branch prediction should hopefully make this not too slow
            CopyNormal(*aivIter, *vIter);
        }

        // TODO: copy rest of data

        ++aivIter;
    }
}

void Importer::CopyIndexData(const AIMeshPtr& aimesh, MeshPtr& mesh)
{
    const aiFace* aifaceIter = aimesh->mFaces;
    IndexList::iterator indexIter = mesh->mIndices.begin();

    const u32 numFaces = aimesh->mNumFaces;
    for (u32 j=0; j < numFaces; ++j)
    {
        CopyIndex(*aifaceIter, indexIter);
        indexIter += 3;
    }
}


void Importer::CopyPosition(const aiVector3D& fromV, Mesh::Vertex& toV, f32 scale)
{
    toV.position.x = fromV.x * scale;
    toV.position.y = fromV.y * scale;
    toV.position.z = fromV.z * scale;
}

void Importer::CopyNormal(const aiVector3D& fromV, Mesh::Vertex& toV)
{
    toV.normal.x = fromV.x;
    toV.normal.y = fromV.y;
    toV.normal.z = fromV.z;
}

void Importer::CopyIndex(const aiFace& face, IndexList::iterator& indexIter)
{
    indexIter[0] = face.mIndices[0];
	indexIter[1] = face.mIndices[1];
	indexIter[2] = face.mIndices[2];
}