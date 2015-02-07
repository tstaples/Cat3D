#ifndef INCLUDED_ENGINE_ANIMATIONCLIP_H
#define INCLUDED_ENGINE_ANIMATIONCLIP_H

//====================================================================================================
// Filename:	AnimationClip.h
// Created by:	Tyler Staples
// Description:	Class representing an animation clip.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Keyframe.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

class BoneAnimation;

//====================================================================================================
// Class Declarations
//====================================================================================================

// AKA AnimationTrack
class AnimationClip
{
public:
    AnimationClip();
    ~AnimationClip();

    std::string mName;

    // Padded to number of bones
    std::vector<BoneAnimation*> mBoneAnimations;

    f32 mDuration;          // Length (time) of animation
    f32 mTicksPerSecond;
    u32 mKeyframeCount;
};

#endif // #ifndef INCLUDED_ENGINE_ANIMATIONCLIP_H