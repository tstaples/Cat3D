//====================================================================================================
// Filename:	Matrix.inl
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Function Definitions
//====================================================================================================

namespace Math
{

inline Vector3 Matrix::GetTranslation() const
{
    return Vector3(_41, _42, _43);
}

//----------------------------------------------------------------------------------------------------

inline Matrix Matrix::Identity()
{
	return Matrix();
}

//----------------------------------------------------------------------------------------------------

inline Matrix Matrix::Translation(f32 x, f32 y, f32 z)
{
	return Matrix
	(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		x,    y,    z,    1.0f
	);
}

//----------------------------------------------------------------------------------------------------

inline Matrix Matrix::Translation(const Vector3& v)
{
	return Matrix
	(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		v.x,  v.y,  v.z,  1.0f
	);
}

//----------------------------------------------------------------------------------------------------

inline Matrix Matrix::RotationX(f32 rad)
{
	const f32 c = cos(rad);
	const f32 s = sin(rad);

	return Matrix
	(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, c,    s,    0.0f,
		0.0f, -s,   c,    0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

//----------------------------------------------------------------------------------------------------

inline Matrix Matrix::RotationY(f32 rad)
{
	const f32 c = cos(rad);
	const f32 s = sin(rad);

	return Matrix
	(
		c,    0.0f, -s,   0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		s,    0.0f, c,    0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

//----------------------------------------------------------------------------------------------------

inline Matrix Matrix::RotationZ(f32 rad)
{
	const f32 c = cos(rad);
	const f32 s = sin(rad);

	return Matrix
	(
		c,    s,    0.0f, 0.0f,
		-s,   c,    0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

//----------------------------------------------------------------------------------------------------

inline Matrix Matrix::Scaling(f32 s)
{
	return Matrix
	(
		s,    0.0f, 0.0f, 0.0f,
		0.0f, s,    0.0f, 0.0f,
		0.0f, 0.0f, s,    0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

//----------------------------------------------------------------------------------------------------

inline Matrix Matrix::Scaling(f32 sx, f32 sy, f32 sz)
{
	return Matrix
	(
		sx,   0.0f, 0.0f, 0.0f,
		0.0f, sy,   0.0f, 0.0f,
		0.0f, 0.0f, sz,   0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

//----------------------------------------------------------------------------------------------------

inline Matrix Matrix::Scaling(const Vector3& s)
{
	return Matrix
	(
		s.x,  0.0f, 0.0f, 0.0f,
		0.0f, s.y,  0.0f, 0.0f,
		0.0f, 0.0f, s.z,  0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

//----------------------------------------------------------------------------------------------------

inline Matrix Matrix::operator-() const
{
	return *this * -1.0f;
}

//----------------------------------------------------------------------------------------------------

inline Matrix Matrix::operator+(const Matrix& rhs) const
{
	return Matrix(_11 + rhs._11, _12 + rhs._12, _13 + rhs._13, _14 + rhs._14,
				  _21 + rhs._21, _22 + rhs._22, _23 + rhs._23, _24 + rhs._24,
				  _31 + rhs._31, _32 + rhs._32, _33 + rhs._33, _34 + rhs._34,
				  _41 + rhs._41, _42 + rhs._42, _43 + rhs._43, _44 + rhs._44);
}

//----------------------------------------------------------------------------------------------------

inline Matrix Matrix::operator-(const Matrix& rhs) const
{
	return Matrix(_11 - rhs._11, _12 - rhs._12, _13 - rhs._13, _14 - rhs._14,
				  _21 - rhs._21, _22 - rhs._22, _23 - rhs._23, _24 - rhs._24,
				  _31 - rhs._31, _32 - rhs._32, _33 - rhs._33, _34 - rhs._34,
				  _41 - rhs._41, _42 - rhs._42, _43 - rhs._43, _44 - rhs._44);
}

//----------------------------------------------------------------------------------------------------

inline Matrix Matrix::operator*(const Matrix& rhs) const
{
	return Matrix
	(
		(_11 * rhs._11) + (_12 * rhs._21) + (_13 * rhs._31) + (_14 * rhs._41),
		(_11 * rhs._12) + (_12 * rhs._22) + (_13 * rhs._32) + (_14 * rhs._42),
		(_11 * rhs._13) + (_12 * rhs._23) + (_13 * rhs._33) + (_14 * rhs._43),
		(_11 * rhs._14) + (_12 * rhs._24) + (_13 * rhs._34) + (_14 * rhs._44),

		(_21 * rhs._11) + (_22 * rhs._21) + (_23 * rhs._31) + (_24 * rhs._41),
		(_21 * rhs._12) + (_22 * rhs._22) + (_23 * rhs._32) + (_24 * rhs._42),
		(_21 * rhs._13) + (_22 * rhs._23) + (_23 * rhs._33) + (_24 * rhs._43),
		(_21 * rhs._14) + (_22 * rhs._24) + (_23 * rhs._34) + (_24 * rhs._44),

		(_31 * rhs._11) + (_32 * rhs._21) + (_33 * rhs._31) + (_34 * rhs._41),
		(_31 * rhs._12) + (_32 * rhs._22) + (_33 * rhs._32) + (_34 * rhs._42),
		(_31 * rhs._13) + (_32 * rhs._23) + (_33 * rhs._33) + (_34 * rhs._43),
		(_31 * rhs._14) + (_32 * rhs._24) + (_33 * rhs._34) + (_34 * rhs._44),

		(_41 * rhs._11) + (_42 * rhs._21) + (_43 * rhs._31) + (_44 * rhs._41),
		(_41 * rhs._12) + (_42 * rhs._22) + (_43 * rhs._32) + (_44 * rhs._42),
		(_41 * rhs._13) + (_42 * rhs._23) + (_43 * rhs._33) + (_44 * rhs._43),
		(_41 * rhs._14) + (_42 * rhs._24) + (_43 * rhs._34) + (_44 * rhs._44)
	);
}

//----------------------------------------------------------------------------------------------------

inline Matrix Matrix::operator*(f32 s) const
{
	return Matrix(_11 * s, _12 * s, _13 * s, _14 * s,
				  _21 * s, _22 * s, _23 * s, _24 * s,
				  _31 * s, _32 * s, _33 * s, _34 * s,
				  _41 * s, _42 * s, _43 * s, _44 * s);
}

//----------------------------------------------------------------------------------------------------

inline Matrix Matrix::operator/(f32 s) const
{
	ASSERT(s != 0.0f, "[Math] Cannot divide by zero!");
	const f32 inv = 1.0f / s;
	return Matrix(_11 * inv, _12 * inv, _13 * inv, _14 * inv,
				  _21 * inv, _22 * inv, _23 * inv, _24 * inv,
				  _31 * inv, _32 * inv, _33 * inv, _34 * inv,
				  _41 * inv, _42 * inv, _43 * inv, _44 * inv);
}

//----------------------------------------------------------------------------------------------------

inline Matrix Matrix::operator+=(const Matrix& rhs)
{
    *this = (*this + rhs);
    return *this;
}

//----------------------------------------------------------------------------------------------------

inline bool operator==(const Matrix& lhs, const Matrix& rhs)
{
    return fabs(lhs._11 - rhs._11) < 0.1f && 
           fabs(lhs._12 - rhs._12) < 0.1f && 
           fabs(lhs._13 - rhs._13) < 0.1f && 
           fabs(lhs._14 - rhs._14) < 0.1f &&
           fabs(lhs._21 - rhs._21) < 0.1f && 
           fabs(lhs._22 - rhs._22) < 0.1f && 
           fabs(lhs._23 - rhs._23) < 0.1f && 
           fabs(lhs._24 - rhs._24) < 0.1f &&
           fabs(lhs._31 - rhs._31) < 0.1f && 
           fabs(lhs._32 - rhs._32) < 0.1f && 
           fabs(lhs._33 - rhs._33) < 0.1f && 
           fabs(lhs._34 - rhs._34) < 0.1f &&
           fabs(lhs._41 - rhs._41) < 0.1f && 
           fabs(lhs._42 - rhs._42) < 0.1f && 
           fabs(lhs._43 - rhs._43) < 0.1f && 
           fabs(lhs._44 - rhs._44) < 0.1f;
}

//----------------------------------------------------------------------------------------------------

inline bool operator!=(const Matrix& lhs, const Matrix& rhs)
{
    return !(lhs == rhs);
}

//----------------------------------------------------------------------------------------------------

inline std::ostream& operator<<(std::ostream& os, const Matrix& m)
{
    // TODO
    return os;
}

} // namespace Math