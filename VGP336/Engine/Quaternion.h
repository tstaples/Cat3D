#ifndef INLCUDED_ENGINE_QUATERNION_H
#define INLCUDED_ENGINE_QUATERNION_H

//====================================================================================================
// Filename:	Quaternion.h
// Created by:	Tyler Staples
// Description:	Struct representing a quaternion.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Vector3.h"

namespace Math
{

//====================================================================================================
// Structs
//====================================================================================================

struct Quaternion
{
    Quaternion();
    Quaternion(f32 x, f32 y, f32 z, f32 w);
    Quaternion(const Vector3& axis, f32 angle);

    Vector3 GetVectorPart() const;
    Quaternion Identity() const;

    Quaternion operator+(const Quaternion& rhs) const;
    Quaternion operator*(const Quaternion& rhs) const;
    Quaternion operator*(f32 scalar) const;

    f32 x, y, z;    // Vector (imaginary part)
    f32 w;          // Real part
};

} // namespace Math

//====================================================================================================
// Inline Definitions
//====================================================================================================

#include "Quaternion.inl"

#endif // #ifndef INLCUDED_ENGINE_QUATERNION_H