#ifndef INCLUDED_ENGINE_MATH_H
#define INCLUDED_ENGINE_MATH_H

//====================================================================================================
// Filename:	EngineMath.h
// Created by:	Peter chan
// Description:	Math library header.
//====================================================================================================

//====================================================================================================
// Defines
//====================================================================================================

#ifdef max
	#undef max
#endif

#ifdef min
	#undef min
#endif

//====================================================================================================
// Includes
//====================================================================================================

#include "Vector2.h"
#include "Vector3.h"
#include "Matrix.h"
#include "ComplexNumber.h"

namespace Math
{

//====================================================================================================
// Constants
//====================================================================================================

extern const f32 kPi;
extern const f32 kTwoPi;
extern const f32 kPiByTwo;
extern const f32 kDegToRad;
extern const f32 kRadToDeg;

//====================================================================================================
// Function Declarations
//====================================================================================================

template <typename T> T Min(T a, T b);
template <typename T> T Max(T a, T b);
template <typename T> T Clamp(T value, T min, T max);

f32 Abs(f32 value);
f32 Sign(f32 value);
f32 Sqr(f32 value);
f32 Sqrt(f32 value);

bool Compare(f32 a, f32 b, f32 epsilon = 0.000001f);

bool IsZero(f32 value);
bool IsZero(const Vector3& v);
<<<<<<< HEAD
bool IsPositive(f32 f);
=======
>>>>>>> c02e68c600a3046aca295e57ef4fcddf0f74138d

f32 MagnitudeSqr(const Vector3& v);
f32 Magnitude(const Vector3& v);

Vector3 Normalize(const Vector3& v);

f32 DistanceSqr(const Vector3& a, const Vector3& b);
f32 Distance(const Vector3& a, const Vector3& b);

f32 Dot(const Vector3& a, const Vector3& b);
Vector3 Cross(const Vector3& a, const Vector3& b);

Vector3 Project(const Vector3& v, const Vector3& n);

f32 Determinant(const Matrix& m);
Matrix Adjoint (const Matrix& m);
Matrix Inverse(const Matrix& m);
Matrix Transpose(const Matrix& m);

Vector3 TransformCoord(const Vector3& v, const Matrix& m);
Vector3 TransformNormal(const Vector3& v, const Matrix& m);

Vector3 Lerp(const Vector3& v0, const Vector3& v1, f32 t);

} // namespace Math

//====================================================================================================
// Inline Definitions
//====================================================================================================

#include "EngineMath.inl"

#endif // #ifndef INCLUDED_ENGINE_MATH_H