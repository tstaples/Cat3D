#ifndef INCLUDED_ENGINE_OBB_H
#define INCLUDED_ENGINE_OBB_H

//====================================================================================================
// Filename:	OBB.h
// Created by:	Tyler Staples
// Description:	Struct for an oriented bounding box.
//====================================================================================================

//====================================================================================================
// Structs
//====================================================================================================

namespace Math
{

struct OBB
{
    Vector3 center;
    Vector3 extend;
    Quaternion rot;

    OBB() 
        : center(0.0f, 0.0f, 0.0f)
        , extend(1.0f, 1.0f, 1.0f)
        , rot(0.0f, 0.0f, 0.0f, 1.0f)
    {}

    /*OBB(const Vector3& _center, const Vector3& _extend, f32 angle)
        : center(_center)
        , extend(_extend)
        , rot(Quaternion::RotationAxis()
    {}*/

    OBB(const Vector3& _center, const Vector3& _extend, const Quaternion& _rot)
        : center(_center)
        , extend(_extend)
        , rot(_rot)
    {}
};

} // namespace Math

#endif // #ifndef INCLUDED_ENGINE_OBB_H