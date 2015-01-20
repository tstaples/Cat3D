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


bool Exporter::Export(const char* outpath, const Meshes& meshes)
{
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
        // Write the number of verts then all the verts
        const u32 numVerts = mesh->GetVertexCount();
        buffer.Write(numVerts);
        buffer.WriteArray(mesh->GetVertices(), numVerts * sizeof(Mesh::Vertex));

        // Write the number of indices and index buffer
        const u32 numIndices = mesh->GetIndexCount();
        buffer.Write(numIndices);
        buffer.WriteArray(mesh->GetIndices(), numIndices * sizeof(u16));
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
        size += mesh->GetVertexCount() * sizeof(Mesh::Vertex);
        size += mesh->GetIndexCount() * sizeof(u16);
    }
    size += sizeof(Header);
    size += sizeof(u32);                 // Number of Meshes
    size += sizeof(u32) * meshes.size(); // Number of verticies
    size += sizeof(u32) * meshes.size(); // Number of indices
    return size;
}