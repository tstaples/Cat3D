#ifndef INCLUDED_ENGINE_KEYFRAME_H
#define INCLUDED_ENGINE_KEYFRAME_H

//====================================================================================================
// Filename:	Keyframe.h
// Created by:	Tyler Staples
// Description:	Class representing an animation keyframe.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "EngineMath.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

// TODO: properly encapsulate eventually
class Keyframe
{
public:
    Keyframe();

    Math::Vector3 mTranslation;
    Math::Quaternion mRotation;
    Math::Vector3 mScale;
    f32 mTime;
};

#endif // #ifndef INCLUDED_ENGINE_KEYFRAME_H