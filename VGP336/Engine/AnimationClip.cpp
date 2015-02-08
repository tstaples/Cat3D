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

AnimationClip::AnimationClip()
{
}

AnimationClip::~AnimationClip()
{
    for (auto boneAnimation : mBoneAnimations)
    {
        delete boneAnimation;
    }
    mBoneAnimations.clear();
}