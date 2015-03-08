//====================================================================================================
// Filename:	Keyframe.h
// Created by:	Tyler Staples
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "Keyframe.h"

Keyframe::Keyframe()
    : mTranslation(Math::Vector3::Zero())
    , mRotation(Math::Quaternion::Identity())
    , mScale(1.0f, 1.0f, 1.0f)
    , mTime(0.0f)
{
}

//----------------------------------------------------------------------------------------------------

Math::Matrix Keyframe::GetTransform() const
{
    Math::Matrix scale = Math::Matrix::Scaling(mScale);
    Math::Matrix rot = Math::Convert(mRotation);
    Math::Matrix translation = Math::Matrix::Translation(mTranslation);
    return (scale * rot * translation);
}