#ifndef INCLUDED_EXPORTER_H
#define INCLUDED_EXPORTER_H

#include "ImporterTypes.h"
#include <iostream>

struct Header
{
    u32 version;
    u32 signature;

    Header(u8 majver, u8 minver, const char* sig)
    {
        version = ((majver << 16) | ('.' << 8) | minver);
        signature = ( (sig[3] << 24) 
                    | (sig[2] << 16) 
                    | (sig[1] << 8) 
                    | (sig[0]));
    }
};

class Exporter
{
public:
    bool Export(const char* outpath, const Meshes& meshes);

private:
    void WriteHeader(const Header& head, std::ofstream& fhandle);
    void WriteVertexBlock(const NativeVertList& verts, std::ofstream& fhandle);
    void WriteIndexBlock(const IndexList& indices, std::ofstream& fhandle);

    void WriteVector3(const Math::Vector3& v, std::ofstream& fhandle);
    void WriteVector2(const Math::Vector2& v, std::ofstream& fhandle);
    void WriteColor(const Color& c, std::ofstream& fhandle);
};

#endif // #ifndef INCLUDED_EXPORTER_H