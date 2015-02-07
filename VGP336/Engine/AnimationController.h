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

class AnimationController
{
    typedef std::vector<Math::Matrix> Transforms;
public:
    AnimationController();
    ~AnimationController();

    void Initialize(Model& model);

    void StartClip(AnimationClip& clip, bool loop);

    const Transforms& GetFinalTransforms() const { return mFinalTransforms; }

private:
    void GetBindPose(Bone* bone);

private:
    AnimationClip* mpCurrentAnimationClip;

    Transforms mToRootTransforms;
    Transforms mFinalTransforms;
    Math::Matrix mInverseRootTransform;

    Model* mpModel;

    f32 mCurrentTime;   // MS
    u32 mCurrentFrame;

    bool mIsPlaying;
    bool mIsLooping;
};

#endif // #ifndef INCLUDED_ENGINE_ANIMATIONCONTROLLER_H