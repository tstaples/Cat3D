#ifndef INCLUDED_ENGINE_ANIMATIONCONTROLLER_H
#define INCLUDED_ENGINE_ANIMATIONCONTROLLER_H

//====================================================================================================
// Filename:	AnimationController.h
// Created by:	Tyler Staples
// Description:	Class for controller animation playback.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "EngineMath.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

struct Bone;
class AnimationClip;
class Model;

//====================================================================================================
// Class Declarations
//====================================================================================================

typedef std::vector<Math::Matrix> Transforms;

class AnimationController
{
public:
    AnimationController();
    ~AnimationController();

    void Initialize(Model& model);

    void StartClip(AnimationClip& clip, bool loop);
    void Update(f32 deltaTime);

    const Transforms& GetFinalTransforms() const    { return mFinalTransforms; }
    const Transforms& ToRootTransforms() const      { return mToRootTransforms; }

private:
    void GetBindPose(Bone* bone);
    void GetPose(f32 time, Bone* bone);

private:
    AnimationClip* mpCurrentAnimationClip;

    Transforms mToRootTransforms;   // Moves bone into root's local space, not root position
    Transforms mFinalTransforms;
    Math::Matrix mInverseRootTransform;

    Model* mpModel;

    f32 mCurrentTime;   // MS
    u32 mCurrentFrame;

    bool mIsPlaying;
    bool mIsLooping;
};

#endif // #ifndef INCLUDED_ENGINE_ANIMATIONCONTROLLER_H