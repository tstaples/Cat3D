#include "Exporter.h"

#include <fstream>
#include "FileBuffer.h"

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

void Testing(const Meshes& meshes)
{
    const size_t size = sizeof(Header)
                        + (sizeof(u32) * 2) // nummeshes + numverts
                        + (sizeof(Mesh::Vertex) * 3); // 3 verts
    // Init our buffer
    FileBuffer outbuffer(size);

    // Write header
    Header header(1, 0, "CATM");
    outbuffer.Write(header);

    // Write how many meshes this file contains
    const u32 numMeshes = meshes.size();
    outbuffer.Write(numMeshes);

    // Write number of verts (fixed size for testing)
    const u32 numVerts = 3;
    outbuffer.Write(numVerts);
    
    // Createa an array of 3 verts and write them to the buffer
    Mesh::Vertex v[numVerts];
    v[0] = meshes[0]->mVertices[0];
    v[1] = meshes[0]->mVertices[1];
    v[2] = meshes[0]->mVertices[2];
    outbuffer.Write(v);
    
    // Get the offset to where the verts start in the buffer and read them
    u32 voffset = outbuffer.GetOffset() - sizeof(v);
    SerialReader reader(outbuffer.GetBuffer() + voffset, outbuffer.Size());
    Mesh::Vertex v2[numVerts];
    reader.Read(v2);
    assert(v2[0].position == v[0].position);

    // Write the buffer to a file
    assert(IO::File::SyncWriteFile("test.catm", outbuffer.GetBuffer(), outbuffer.Size()));

    // ### READING ###

    // init a buffer with the contents of the file
    FileBuffer inbuffer("test.catm");
    assert(inbuffer.Initialized());
    
    // Read the file and create a serialreader. check header is correcet
    SerialReader sin(inbuffer.GetBuffer(), inbuffer.Size());
    std::string sig = sin.ReadFormatted(4);
    assert(sig.compare("CATM") == 0);

    // Ensure the version number is correct
    u32 ver = 0;
    sin.Read(ver);
    std::pair<u8, u8> version = Header::GetVersion(ver);
    assert(version.first == 1 && version.second == 0);

    // Read number of meshes and verts
    u32 numMeshesinfile = sin.Read<u32>();
    assert(numMeshesinfile == numMeshes);
    u32 numVertsinfile = sin.Read<u32>();
    assert(numVertsinfile == numVerts);
 
    // Read the data from the buffer into the array
    Mesh::Vertex vFromFile[numVerts];
    sin.Read(vFromFile);
    assert(vFromFile[0].position == v[0].position);
}

bool Exporter::Export(const char* outpath, const Meshes& meshes)
{
    Testing(meshes);

    // Get total size of all mesh data
    size_t size = CalculateSize(meshes);;

    // Init our buffer
    FileBuffer buffer(size);

    // Write header (TODO: read version from config)
    Header header(1, 0, "CATM");
    buffer.Write(header);

    // Write how many meshes this file contains
    const u32 numMeshes = meshes.size();
    buffer.Write(numMeshes);

    // Write all the vertex and index data
    for (auto &mesh : meshes)
    {
        WriteVertexBlock(mesh->mVertices, buffer);
        WriteIndexBlock(mesh->mIndices, buffer);
    }

    // Output the buffer
    bool rc = false;
    if (IO::File::SyncWriteFile(outpath, buffer.GetBuffer(), size))
    {
        rc = true;
    }
    buffer.Clear();
    return rc;
}

size_t Exporter::CalculateSize(const Meshes& meshes)
{
    size_t size = 0;
    for (auto& mesh : meshes)
    {
        size += mesh->GetSize();
    }
    size += sizeof(Header);
    size += sizeof(u32);                 // Number of Meshes
    size += sizeof(u32) * meshes.size(); // Number of verticies
    size += sizeof(u32) * meshes.size(); // Number of indices
    return size;
}

void Exporter::WriteVertexBlock(const NativeVertList& verts, FileBuffer& buffer)
{
    // Write how many verticies are following
    const u32 numVerts = verts.size();
    buffer.Write(numVerts);

    for (auto v : verts)
    {
        buffer.Write(v.position);
        buffer.Write(v.normal);
        buffer.Write(v.tangent);
        buffer.Write(v.color);
        buffer.Write(v.texcoord);
    }
}

void Exporter::WriteIndexBlock(const IndexList& indices, FileBuffer& buffer)
{
    // Write how many indices are following
    const u32 numIndices = indices.size();
    buffer.Write(numIndices);

    for (auto i : indices)
    {
        buffer.Write(i);
    }
}