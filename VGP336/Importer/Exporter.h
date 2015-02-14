#ifndef INCLUDED_EXPORTER_H
#define INCLUDED_EXPORTER_H

#include "ImporterTypes.h"
#include <iostream>

class FileBuffer;

struct Header
{
    u32 signature;
    u32 version;

    Header(u8 majver, u8 minver, const char* sig)
    {
        version = ((majver << 16) | ('.' << 8) | minver);
        signature = ( (sig[3] << 24) 
                    | (sig[2] << 16) 
                    | (sig[1] << 8) 
                    | (sig[0]));
    }

    inline static std::pair<u8, u8> GetVersion(u32 v)
    {
        u8 maj = (v >> 16);
        u8 min = (v << 24);
        return std::make_pair(maj, min);
    }
};

class Exporter
{
    typedef std::vector<Mesh*> Meshes;
    typedef std::vector<std::string> StringVec;
    typedef std::vector<Bone*> BoneVec;
    typedef std::vector<AnimationClip*> Animations;

public:
    bool Export(const char* outpath, 
                const Meshes& meshes, 
                const StringVec& texPaths, 
                const BoneVec& bones,
                const Animations& animations);

private:
    size_t CalculateSize(const Meshes& meshes, const StringVec& texPaths, const BoneVec& bones, const Animations& animations);

    void ExportMeshes(const Meshes& meshes, FileBuffer& buffer);
    void ExportTextures(const StringVec& texPaths, FileBuffer& buffer);
    void ExportBones(const BoneVec& bones, FileBuffer& buffer);
    void ExportBoneWeights(const Meshes& meshes, FileBuffer& buffer);
    void ExportAnimations(const Animations& animations, FileBuffer& buffer);

    void WriteLengthEncodedString(const std::string& str, FileBuffer& buffer);
};

#endif // #ifndef INCLUDED_EXPORTER_H