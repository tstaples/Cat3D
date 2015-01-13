#include "Exporter.h"

#include <fstream>

/* === Format Guidelines ===
 * Layout:
 *	Header size (fixed): 8 bytes
 *	Number of meshes (4 bytes)
 *	Vertex block:
 *		Number of verts (4 bytes)
 *		- all values for each property space delimited on single line
 *		Position: x y z (12)
 *		Normal: x y z (12)
 *		Tangent: x y z (12)
 *		Color: r g b a (16)
 *		Texture Coords: u v (8)
 *	Indices block:
 *		Number of indicies (4)
 *		- each index is newline (\r\n) delimited (4)
 * 
*/

// TODO: write all data as a contiguous block of bytes that can just be read in to memory
// then when loading the native format, blocks of the data can just be memcpy'd

template<typename T>
void Write(std::ofstream& of, T* data, size_t size)
{
    of.write((char*)data, sizeof(T) * size);
}

bool Exporter::Export(const char* outpath, const Meshes& meshes)
{
    std::ofstream fout(outpath, std::ios::binary | std::ios::out);
    if (!fout.is_open())
    {
        return false;
    }

    // Write header (TODO: read version from config)
    Header header(1, 0, "CATM");
    WriteHeader(header, fout);

    // Write how many meshes this file contains
    const u32 numMeshes = meshes.size();
    Write(fout, &numMeshes, 1);

    for (auto &mesh : meshes)
    {
        WriteVertexBlock(mesh->mVertices, fout);
        WriteIndexBlock(mesh->mIndices, fout);
    }

    fout.close();
    return true;
}

void Exporter::WriteHeader(const Header& head, std::ofstream& fhandle)
{
    Write(fhandle, &head.signature, 1);
    Write(fhandle, &head.version, 1);
}

void Exporter::WriteVertexBlock(const NativeVertList& verts, std::ofstream& fhandle)
{
    // Write how many verticies are following
    const u32 numVerts = verts.size();
    Write(fhandle, &numVerts, 1);

    for (auto v : verts)
    {
        WriteVector3(v.position, fhandle);
        WriteVector3(v.normal, fhandle);
        WriteVector3(v.tangent, fhandle);
        WriteColor(v.color, fhandle);
        WriteVector2(v.texcoord, fhandle);
    }
}

void Exporter::WriteIndexBlock(const IndexList& indices, std::ofstream& fhandle)
{
    // Write how many indices are following
    const u32 numIndices = indices.size();
    Write(fhandle, &numIndices, 1);

    for (auto i : indices)
    {
        Write(fhandle, &i, 1);
    }
}

void Exporter::WriteVector3(const Math::Vector3& v, std::ofstream& fhandle)
{
    std::string s(S(v.x) + " " + S(v.y) + " " + S(v.z));
    const std::size_t sz = s.size();

    const s8* buff = s.c_str();;
    Write(fhandle, buff, sz);
}

void Exporter::WriteVector2(const Math::Vector2& v, std::ofstream& fhandle)
{
    std::string s(S(v.x) + " " + S(v.y));
    const std::size_t sz = s.size();

    const s8* buff = s.c_str();;
    Write(fhandle, buff, sz);
}

void Exporter::WriteColor(const Color& c, std::ofstream& fhandle)
{
    std::string s(S(c.r) + " " + S(c.g) + " " + S(c.b) + " " + S(c.a));
    const std::size_t sz = s.size();

    const s8* buff = s.c_str();;
    Write(fhandle, buff, sz);
}
