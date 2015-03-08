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

//----------------------------------------------------------------------------------------------------

BoneAnimation::~BoneAnimation()
{
    for (auto keyframe : mKeyframes)
    {
        delete keyframe;
    }
    mKeyframes.clear();
}

//----------------------------------------------------------------------------------------------------

Math::Matrix BoneAnimation::GetTransform(f32 time) const
{
    // get 2 frames, lerp between them to get interpolated 3 values
    // concatinate 3 values (position, rotation, scale), and return final matrix
    ASSERT(!mKeyframes.empty(), "[BoneAnimation] Keyframes is Empty!");

    if(time < mKeyframes[0]->mTime)
    {
        // return first keyframe data, no lerping
        return mKeyframes[0]->GetTransform();
    }
    else if(time >= mKeyframes.back()->mTime)
    {
        // return last keyframe data, no lerping
        const u32 index = mKeyframes.size() - 1;
        return mKeyframes[index]->GetTransform();
    }

    const u32 index = FindKeyframeIndex(time);

    // Store current and next keyframes
    const Keyframe* keyframe = mKeyframes[index];
    const Keyframe* nextKeyframe = mKeyframes[index + 1];

    const f32 keyframeTime = keyframe->mTime;
    f32 t = ((time - keyframeTime) / (nextKeyframe->mTime - keyframeTime));

    // lerp between the keyframe at the current index and the next index
    Math::Matrix TranslationMatrix   = Math::Matrix::Translation(Math::Lerp(keyframe->mTranslation, nextKeyframe->mTranslation, t));
    Math::Matrix RotationMatrix      = Math::Convert(Math::Slerp(keyframe->mRotation, nextKeyframe->mRotation, t));
    Math::Matrix ScaleMatrix         = Math::Matrix::Scaling(Math::Lerp(keyframe->mScale, nextKeyframe->mScale, t));

    return (ScaleMatrix * RotationMatrix * TranslationMatrix);
}

//----------------------------------------------------------------------------------------------------

u32 BoneAnimation::FindKeyframeIndex(f32 time) const
{
    u32 keyframeSize = mKeyframes.size();  
    ASSERT(!mKeyframes.empty(), "[BoneAnimation] Keyframes is Empty!");

    for(u32 i = 0; i < keyframeSize; ++i)
    {
        if(time < mKeyframes[i]->mTime)
        {
            if(i > 0)
            {
                return i-1;
            }
            else
            {
                return i;
            }
        }
    }
    return keyframeSize-1;
}