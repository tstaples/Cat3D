//====================================================================================================
// Filename:	BoneAnimation.h
// Created by:	Tyler Staples
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "BoneAnimation.h"

BoneAnimation::BoneAnimation()
    : mpBone(nullptr)
    , mBoneIndex(0)
{
}

BoneAnimation::~BoneAnimation()
{
    for (auto keyframe : mKeyframes)
    {
        delete keyframe;
    }
    mKeyframes.clear();
}