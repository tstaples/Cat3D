#ifndef INCLUDED_ENGINE_BONEANIMATION_H
#define INCLUDED_ENGINE_BONEANIMATION_H

//====================================================================================================
// Filename:	BoneAnimation.h
// Created by:	Tyler Staples
// Description:	Class for holding animation keyframes for a bone.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Keyframe.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

struct Bone;

//====================================================================================================
// Class Declarations
//====================================================================================================

// AKA AnimationTrack
class BoneAnimation
{
public:
    BoneAnimation();
    ~BoneAnimation();

    Math::Matrix GetTransform(f32 time) const;
    u32 FindKeyframeIndex(f32 time) const;

    std::vector<Keyframe*> mKeyframes;
    Bone* mpBone;
    u32 mBoneIndex;
};

#endif // #ifndef INCLUDED_ENGINE_BONEANIMATION_H