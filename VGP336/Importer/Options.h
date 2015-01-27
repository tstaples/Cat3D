#ifndef INCLUDED_IMPORTER_OPTIONS_H
#define INCLUDED_IMPORTER_OPTIONS_H

#include <assimp/postprocess.h>

#include <map>

namespace Options
{
    typedef std::map<const char*, u32> OptMap;

    // Individual options
    static OptMap flagMap = 
    {
        { "CalcTangentSpace"    , aiProcess_CalcTangentSpace },
        { "JoinIdenticalVerts"  , aiProcess_JoinIdenticalVertices },
        { "MakeLeftHanded"      , aiProcess_MakeLeftHanded },
        { "Triangulate"         , aiProcess_Triangulate },
        { "RemoveComponent"     , aiProcess_RemoveComponent },
        { "GenNormals"          , aiProcess_GenNormals },
        { "GenSmoothNormals"    , aiProcess_GenSmoothNormals },
        { "SplitLargMeshes"     , aiProcess_SplitLargeMeshes },
        { "PreTransformVerts"   , aiProcess_PreTransformVertices },
        { "LimitBoneWeight"     , aiProcess_LimitBoneWeights },
        { "ValidateDS"          , aiProcess_ValidateDataStructure },
        { "ImproveCacheLoc"     , aiProcess_ImproveCacheLocality },
        { "RemoveRedundantMats" , aiProcess_RemoveRedundantMaterials },
        { "FixInfacingNorms"    , aiProcess_FixInfacingNormals },
        { "SortByType"          , aiProcess_SortByPType },
        { "FindDegenerates"     , aiProcess_FindDegenerates },
        { "FindInvalidData"     , aiProcess_FindInvalidData },
        { "GenUVCoords"         , aiProcess_GenUVCoords },
        { "TransformUVCoords"   , aiProcess_TransformUVCoords },
        { "FindInstances"       , aiProcess_FindInstances },
        { "OptimizeMeshes"      , aiProcess_OptimizeMeshes },
        { "OptimizeGraph"       , aiProcess_OptimizeGraph },
        { "FlipUVs"             , aiProcess_FlipUVs },
        { "FlipWindOrder"       , aiProcess_FlipWindingOrder },
        { "SplitByBoneCount"    , aiProcess_SplitByBoneCount },
        { "Debone"              , aiProcess_Debone }
    };

    // Flags that indicate the combination of multiple flags
    static OptMap flagGroups = 
    {
        { "ConvertToLeftHanded" , aiProcess_ConvertToLeftHanded },
        { "TargetRT_Fast"       , aiProcessPreset_TargetRealtime_Fast },
        { "TargetRT_Quality"    , aiProcessPreset_TargetRealtime_Quality },
        { "TargetRT_MaxQuality" , aiProcessPreset_TargetRealtime_MaxQuality }
    };
}

#endif // #ifndef INCLUDED_IMPORTER_OPTIONS_H