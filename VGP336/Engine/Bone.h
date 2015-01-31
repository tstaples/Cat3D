#ifndef INCLUDED_ENGINE_BONE_H
#define INCLUDED_ENGINE_BONE_H

//====================================================================================================
// Filename:	Bone.h
// Created by:	Tyler Staples
// Description:	Structs for a bone
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "EngineMath.h"

//====================================================================================================
// Structs
//====================================================================================================

#define NO_PARENT -1

struct Bone
{
    Bone()
        : parent(nullptr)
        , parentIndex(NO_PARENT)
        , index(0)
    {}

    std::string name;

    Bone* parent;
    std::vector<Bone*> children;

    // Used for when saving the model
    s32 parentIndex;
    std::vector<u32> childrenIndices;

    Math::Matrix transform;         // Relative to parent
    Math::Matrix offsetTransform;   // Offset from origin

    u32 index; // Position in the bone array
};

#endif // #ifndef INCLUDED_ENGINE_BONE_H