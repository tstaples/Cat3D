//====================================================================================================
// Filename:	AnimationClip.h
// Created by:	Tyler Staples
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "AnimationClip.h"
#include "BoneAnimation.h"
#include "Bone.h"

AnimationClip::AnimationClip()
{
}

//----------------------------------------------------------------------------------------------------

AnimationClip::~AnimationClip()
{
    for (auto boneAnimation : mBoneAnimations)
    {
        delete boneAnimation;
    }
    mBoneAnimations.clear();
}

//----------------------------------------------------------------------------------------------------

Math::Matrix AnimationClip::GetTransform(f32 time, Bone* bone) const
{
    Math::Matrix transform;
    BoneAnimation* boneAnim = mBoneAnimations[bone->index];
    if(boneAnim != nullptr)
    {
        transform = boneAnim->GetTransform(time);
    }
    return transform;
}