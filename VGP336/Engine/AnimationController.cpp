//====================================================================================================
// Filename:	AnimationController.h
// Created by:	Tyler Staples
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "AnimationController.h"

#include "AnimationClip.h"
#include "Bone.h"
#include "Model.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

AnimationController::AnimationController()
    : mpModel(nullptr)
    , mCurrentTime(0.0f)
    , mCurrentFrame(0)
    , mIsPlaying(false)
    , mIsLooping(false)
{
}

//----------------------------------------------------------------------------------------------------

AnimationController::~AnimationController()
{
}

//----------------------------------------------------------------------------------------------------

void AnimationController::Initialize(Model& model)
{
    mpModel = &model;

    // Resize to the number of bones the model contains
    mToRootTransforms.resize(model.mBones.size());
    mFinalTransforms.resize(model.mBones.size());
}

//----------------------------------------------------------------------------------------------------

void AnimationController::StartClip(AnimationClip& clip, bool loop)
{
    // Reset values
    mpCurrentAnimationClip  = &clip;
    mCurrentTime            = 0.0f;
    mCurrentFrame           = 0;
    mIsPlaying              = true;
    mIsLooping              = loop;

    // Calculate and store inverse root transform
    mInverseRootTransform = Math::Inverse(mpModel->mpRoot->transform);
    GetBindPose(mpModel->mpRoot);
}

//----------------------------------------------------------------------------------------------------

void AnimationController::Update(f32 deltaTime)
{
    if (!mIsPlaying)
    {
        return;
    }

    mCurrentTime += deltaTime * mpCurrentAnimationClip->mTicksPerSecond;
    if(mCurrentTime >= mpCurrentAnimationClip->mDuration)
    {
        if (mIsLooping)
        {
            mCurrentTime = 0.0f;
        }
        else
        {
            mIsPlaying = false;
        }
    }
 
    if (mIsPlaying)
    {
        GetPose(mCurrentTime, mpModel->mpRoot);
    }
}

//----------------------------------------------------------------------------------------------------

void AnimationController::GetBindPose(Bone* bone)
{
    Math::Matrix toParentTransform = bone->transform;
    Math::Matrix toRootTransform = toParentTransform;

    if (bone->parent)
    {
        toRootTransform = toParentTransform * mToRootTransforms[bone->parent->index];
    }

    mToRootTransforms[bone->index]  = toRootTransform;
    mFinalTransforms[bone->index]   = bone->offsetTransform     // Bone space
                                    * toRootTransform           // Root space
                                    * mInverseRootTransform;    // Worth space

    for (Bone* child : bone->children)
    {
        GetBindPose(child);
    }
}

//----------------------------------------------------------------------------------------------------

void AnimationController::GetPose(f32 time, Bone* bone)
{
    // dont use toParent. check based on current Animation clip
    Math::Matrix toParentTransform;// = bone->transform;  // allows bone to go to parent's space
    toParentTransform = mpCurrentAnimationClip->GetTransform(time, bone); // Get transform for that bone at particular time
       
    Math::Matrix toRootTransform = toParentTransform;
    if (bone->parent != nullptr)
    {
        toRootTransform = toParentTransform * mToRootTransforms[bone->parent->index];
    }
       
    mToRootTransforms[bone->index] = toRootTransform;
    mFinalTransforms[bone->index] = bone->offsetTransform * toRootTransform * mInverseRootTransform;
       
    for (Bone* child : bone->children)
    {
        GetPose(time, child);
    }
}