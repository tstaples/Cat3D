namespace Math
{

inline ComplexNumber::ComplexNumber()
: mReal(0.0f)
, mImaginary(0.0f)
{
}
//----------------------------------------------------------------------------------------------------

inline ComplexNumber::ComplexNumber(f32 real, f32 imaginary)
: mReal(real)
, mImaginary(imaginary)
{
}
//----------------------------------------------------------------------------------------------------

inline ComplexNumber::ComplexNumber(const ComplexNumber& rhs)
{
	*this = rhs;
}

//----------------------------------------------------------------------------------------------------

inline ComplexNumber& ComplexNumber::operator=(const ComplexNumber& rhs)
{
	if (this != &rhs)
	{
		mReal = rhs.mReal;
		mImaginary = rhs.mImaginary;
	}
	return *this;
}

//----------------------------------------------------------------------------------------------------

inline f32 ComplexNumber::Modulus() const
{
	// "Distance from zero" aka length
	return Sqrt((mReal * mReal) + (mImaginary * mImaginary));
}

//----------------------------------------------------------------------------------------------------

inline ComplexNumber ComplexNumber::Conjugate() const
{
	// Flip sign of imaginary part
	return ComplexNumber(mReal, mImaginary * -1.0f);
}

//----------------------------------------------------------------------------------------------------

inline f32 ComplexNumber::Argument() const
{
	if (IsZero(mReal) || IsZero(mImaginary))
		return 0.0f;
	return atan2f(mImaginary, mReal);
}

//----------------------------------------------------------------------------------------------------

inline ComplexNumber operator+(const ComplexNumber& lhs, const ComplexNumber& rhs)
{
	return ComplexNumber(lhs.mReal + rhs.mReal, lhs.mImaginary + rhs.mImaginary);
}

//----------------------------------------------------------------------------------------------------

inline ComplexNumber operator+(const ComplexNumber& lhs, f32 scalar)
{
	return ComplexNumber(lhs.mReal + scalar, lhs.mImaginary);
}

//----------------------------------------------------------------------------------------------------

inline ComplexNumber operator-(const ComplexNumber& lhs, const ComplexNumber& rhs)
{
	return ComplexNumber(lhs.mReal - rhs.mReal, lhs.mImaginary - rhs.mImaginary);
}

//----------------------------------------------------------------------------------------------------

inline ComplexNumber operator-(const ComplexNumber& lhs, f32 scalar)
{
	return ComplexNumber(lhs.mReal - scalar, lhs.mImaginary);
}

//----------------------------------------------------------------------------------------------------

inline ComplexNumber operator*(const ComplexNumber& lhs, const ComplexNumber& rhs)
{
	f32 first = lhs.mReal * rhs.mReal;
	f32 outside = lhs.mReal * rhs.mImaginary;
	f32 inside = lhs.mImaginary * rhs.mReal;
	f32 last = lhs.mImaginary * rhs.mImaginary * -1.0f; // i^2 = -1
	return ComplexNumber (first + last, outside + inside);
}

//----------------------------------------------------------------------------------------------------

inline ComplexNumber operator*(const ComplexNumber& lhs, f32 scalar)
{
	return ComplexNumber(lhs.mReal * scalar, lhs.mImaginary * scalar);
}

//----------------------------------------------------------------------------------------------------

inline ComplexNumber operator/(const ComplexNumber& lhs, const ComplexNumber& rhs)
{
	return ComplexNumber(lhs * rhs.Conjugate() / Sqr(rhs.Modulus()));
}

//----------------------------------------------------------------------------------------------------

inline ComplexNumber operator/(const ComplexNumber& lhs, f32 scalar)
{
	return ComplexNumber(lhs.mReal / scalar, lhs.mImaginary / scalar);
}

//----------------------------------------------------------------------------------------------------

inline bool operator==(const ComplexNumber& lhs, const ComplexNumber& rhs)
{
	return (lhs.mReal == rhs.mReal && lhs.mImaginary == rhs.mImaginary);
}

//----------------------------------------------------------------------------------------------------

inline bool operator==(const ComplexNumber& lhs, f32 scalar)
{
	return (lhs.mReal == scalar && IsZero(lhs.mImaginary));
}

//----------------------------------------------------------------------------------------------------

inline bool operator!=(const ComplexNumber& lhs, const ComplexNumber& rhs)
{
	return !(lhs == rhs);
}

//----------------------------------------------------------------------------------------------------

inline bool operator!=(const ComplexNumber& lhs, f32 scalar)
{
	return !(lhs == scalar);
}

} // namespace Math