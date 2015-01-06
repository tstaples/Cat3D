//====================================================================================================
// Filename:	Vector3.inl
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Namespace
//====================================================================================================

namespace Math
{

//====================================================================================================
// Function Definitions
//====================================================================================================
	
inline Vector3 Vector3::Zero()
{
	return Vector3();
}

//----------------------------------------------------------------------------------------------------

inline Vector3 Vector3::XAxis()
{
	return Vector3(1.0f, 0.0f, 0.0f);
}

//----------------------------------------------------------------------------------------------------

inline Vector3 Vector3::YAxis()
{
	return Vector3(0.0f, 1.0f, 0.0f);
}

//----------------------------------------------------------------------------------------------------

inline Vector3 Vector3::ZAxis()
{
	return Vector3(0.0f, 0.0f, 1.0f);
}

//----------------------------------------------------------------------------------------------------

inline Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

//----------------------------------------------------------------------------------------------------

inline Vector3 Vector3::operator+(const Vector3& rhs) const
{
	return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
}

//----------------------------------------------------------------------------------------------------

inline Vector3 Vector3::operator-(const Vector3& rhs) const
{
	return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
}

//----------------------------------------------------------------------------------------------------

inline Vector3 Vector3::operator*(f32 s) const
{
	return Vector3(x * s, y * s, z * s);
}

//----------------------------------------------------------------------------------------------------

inline Vector3 Vector3::operator/(f32 s) const
{
	ASSERT(s != 0.0f, "[Math] Cannot divide by zero!");
	const f32 inv = 1.0f / s;
	return Vector3(x * inv, y * inv, z * inv);
}

//----------------------------------------------------------------------------------------------------

inline Vector3& Vector3::operator+=(const Vector3& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

//----------------------------------------------------------------------------------------------------

inline Vector3& Vector3::operator-=(const Vector3& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

//----------------------------------------------------------------------------------------------------

inline Vector3& Vector3::operator*=(f32 s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

//----------------------------------------------------------------------------------------------------

inline Vector3& Vector3::operator/=(f32 s)
{
	ASSERT(s != 0.0f, "[Math] Cannot divide by zero!");
	const f32 inv = 1.0f / s;
	x *= inv;
	y *= inv;
	z *= inv;
	return *this;
}

} // namespace Math