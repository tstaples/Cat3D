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
        version = ((majver << 8) | minver);
        signature = ( (sig[0] << 24) 
                    | (sig[1] << 16) 
                    | (sig[2] << 8) 
                    | sig[3]);
    }

    friend std::ostream& operator<<(std::ostream& os, const Header& head)
    {
        os << head.signature << std::endl << head.signature << std::endl;
        return os;
    }
};

class Exporter
{
public:
    bool Export(const char* outpath, const Meshes& meshes);

private:
    void WriteVertexBlock(const NativeVertList& verts, std::ofstream& fhandle);
    void WriteIndexBlock(const IndexList& indices, std::ofstream& fhandle);

    std::string FormatVector3(const Math::Vector3& v);
    std::string FormatVector2(const Math::Vector2& v);
    std::string FormatColor(const Color& c);
};

#endif // #ifndef INCLUDED_EXPORTER_H