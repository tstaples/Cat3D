#ifndef INCLUDED_ENGINE_COMPLEXNUMBER_H
#define INCLUDED_ENGINE_COMPLEXNUMBER_H

//====================================================================================================
// Filename:	ComplexNumber.h
// Created by:	Tyler Staples
// Description:	Class representing a complex number.
//====================================================================================================

#include "Common.h"
#include "EngineMath.h"

namespace Math
{

class ComplexNumber
{
	friend ComplexNumber operator+(const ComplexNumber& lhs, const ComplexNumber& rhs);
	friend ComplexNumber operator+(const ComplexNumber& lhs, f32 scalar);
	friend ComplexNumber operator-(const ComplexNumber& lhs, const ComplexNumber& rhs);
	friend ComplexNumber operator-(const ComplexNumber& lhs, f32 scalar);
	friend ComplexNumber operator*(const ComplexNumber& lhs, const ComplexNumber& rhs);
	friend ComplexNumber operator*(const ComplexNumber& lhs, f32 scalar);
	friend ComplexNumber operator/(const ComplexNumber& lhs, const ComplexNumber& rhs);
	friend ComplexNumber operator/(const ComplexNumber& lhs, f32 scalar);

	friend bool operator==(const ComplexNumber& lhs, const ComplexNumber& rhs);
	friend bool operator==(const ComplexNumber& lhs, f32 scalar);
	friend bool operator!=(const ComplexNumber& lhs, const ComplexNumber& rhs);
	friend bool operator!=(const ComplexNumber& lhs, f32 scalar);

public:
	ComplexNumber();
	ComplexNumber(f32 real, f32 imaginary);
	ComplexNumber(const ComplexNumber& rhs);
	ComplexNumber& operator=(const ComplexNumber& rhs);

	f32 Modulus() const;
	f32 Argument() const;
	ComplexNumber Conjugate() const;

private:
	f32 mReal;
	f32 mImaginary;
};

} // namespace Math

//====================================================================================================
// Inline Definitions
//====================================================================================================

#include "ComplexNumber.inl"

#endif // #ifndef INCLUDED_ENGINE_COMPLEXNUMBER_H