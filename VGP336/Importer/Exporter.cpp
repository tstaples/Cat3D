#include "Exporter.h"

#include <fstream>

/* === Format Guidelines ===
 * Layout:
 * 
*/

template<typename T>
std::string S(const T& tostr)
{
    return std::to_string(tostr);
}

bool Exporter::Export(const char* outpath, const Meshes& meshes)
{
    std::ofstream fout(outpath);
    if (!fout.is_open())
    {
        return false;
    }

    // Write header (TODO: read version from config)
    Header header(1, 0, "CATM");
    fout << header;

    // Write how many meshes this file contains
    const u32 numMeshes = meshes.size();
    fout << numMeshes << std::endl;

    for (auto &mesh : meshes)
    {
        WriteVertexBlock(mesh->mVertices, fout);
        WriteIndexBlock(mesh->mIndices, fout);
    }

    fout.close();
    return true;
}

void Exporter::WriteVertexBlock(const NativeVertList& verts, std::ofstream& fhandle)
{
    // Write how many verticies are following
    const u32 numVerts = verts.size();
    fhandle << numVerts << std::endl;

    for (auto v : verts)
    {
        fhandle << FormatVector3(v.position) << std::endl;
        fhandle << FormatVector3(v.normal) << std::endl;
        fhandle << FormatVector3(v.tangent) << std::endl;
        fhandle << FormatColor(v.color) << std::endl;
        fhandle << FormatVector2(v.texcoord) << std::endl;
    }
}

void Exporter::WriteIndexBlock(const IndexList& indices, std::ofstream& fhandle)
{
    // Write how many indices are following
    const u32 numIndices = indices.size();
    fhandle << numIndices << std::endl;

    for (auto i : indices)
    {
        fhandle << i << std::endl;
    }
}

std::string Exporter::FormatVector3(const Math::Vector3& v)
{
    return std::string(S(v.x) + " " + S(v.y) + " " + S(v.z));
}

std::string Exporter::FormatVector2(const Math::Vector2& v)
{
    return std::string(S(v.x) + " " + S(v.y));
}

std::string Exporter::FormatColor(const Color& c)
{
    return std::string(S(c.r) + " " + S(c.g) + " " + S(c.b) + " " + S(c.a));
}
