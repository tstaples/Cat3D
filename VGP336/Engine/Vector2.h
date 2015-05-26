#ifndef INCLUDED_ENGINE_VECTOR2_H
#define INCLUDED_ENGINE_VECTOR2_H

//====================================================================================================
// Filename:	Vector2.h
// Created by:	Peter chan
// Description:	Struct for a 2D vector.
//====================================================================================================

//====================================================================================================
// Structs
//====================================================================================================

namespace Math
{

struct Vector2
{
	f32 x, y;

	Vector2() : x(0.0f), y(0.0f) {}
	Vector2(f32 x, f32 y) : x(x), y(y) {}

    static Vector2 Zero();
	static Vector2 XAxis();
	static Vector2 YAxis();

	Vector2 operator-() const;
	Vector2 operator+(const Vector2& rhs) const;
	Vector2 operator-(const Vector2& rhs) const;
	Vector2 operator*(f32 s) const;
	Vector2 operator/(f32 s) const;

	Vector2& operator+=(const Vector2& rhs);
	Vector2& operator-=(const Vector2& rhs);
	Vector2& operator*=(f32 s);
	Vector2& operator/=(f32 s);

    friend bool operator==(const Vector2& lhs, const Vector2& rhs);
    friend bool operator!=(const Vector2& lhs, const Vector2& rhs);
};

} // namespace Math

#include "Vector2.inl"

#endif // #ifndef INCLUDED_MATH_VECTOR2_H