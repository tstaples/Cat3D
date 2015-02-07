//====================================================================================================
// Filename:	AnimationClip.h
// Created by:	Tyler Staples
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "AnimationClip.h"

AnimationClip::AnimationClip()
{
}

AnimationClip::~AnimationClip()
{
    SafeDeleteSTL(mBoneAnimations);
    /*for (auto boneAnimation : mBoneAnimations)
    {
        delete boneAnimation;
    }
    mBoneAnimations.clear();*/
}