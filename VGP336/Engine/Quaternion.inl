//====================================================================================================
// Filename:	Quaternion.h
// Created by:	Tyler Staples
//====================================================================================================

//====================================================================================================
// Function Definitions
//====================================================================================================

namespace Math
{

inline Quaternion::Quaternion()
    : x(0.0f)
    , y(0.0f)
    , z(0.0f)
    , w(1.0f)
{
}

//----------------------------------------------------------------------------------------------------

inline Quaternion::Quaternion(f32 x, f32 y, f32 z, f32 w)
    : x(x)
    , y(y)
    , z(z)
    , w(w)
{
}

//----------------------------------------------------------------------------------------------------

inline Quaternion::Quaternion(const Vector3& axis, f32 angle)
{
    //q = (sin(theta/2) * v3) + cos(theta/2)
    x = axis.x * sin(angle * 0.5f);
    y = axis.y * sin(angle * 0.5f);
    z = axis.z * sin(angle * 0.5f);
    w = cos(angle * 0.5f);
}

//----------------------------------------------------------------------------------------------------

inline Quaternion Quaternion::Identity()
{
    return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
}

//----------------------------------------------------------------------------------------------------

inline Quaternion Quaternion::Pure(const Vector3& v)
{
    return Quaternion(v.x, v.y, v.z, 0.0f);
}

//----------------------------------------------------------------------------------------------------

inline Vector3 Quaternion::GetVectorPart() const
{
    return Vector3(x, y, z);
}

//----------------------------------------------------------------------------------------------------

inline Quaternion Quaternion::operator+(const Quaternion& rhs) const
{
    return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
}

//----------------------------------------------------------------------------------------------------

inline Quaternion Quaternion::operator*(const Quaternion& rhs) const
{
    Quaternion q;
    q.w = (rhs.w * w) - (rhs.x * x) - (rhs.y * y) - (rhs.z * z);
	q.x = (rhs.w * x) + (rhs.x * w) + (rhs.y * z) - (rhs.z * y);
	q.y = (rhs.w * y) + (rhs.y * w) + (rhs.z * x) - (rhs.x * z);
	q.z = (rhs.w * z) + (rhs.z * w) + (rhs.x * y) - (rhs.y * x);
    return q;
}

//----------------------------------------------------------------------------------------------------

inline Quaternion Quaternion::operator*(f32 scalar) const
{
    return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar);
}

//----------------------------------------------------------------------------------------------------

inline bool operator==(const Quaternion& lhs, const Quaternion& rhs)
{
    return ((lhs.x == rhs.x) &&
            (lhs.y == rhs.y) &&
            (lhs.z == rhs.z) &&
            (lhs.w == rhs.w));
}

//----------------------------------------------------------------------------------------------------

inline bool operator!=(const Quaternion& lhs, const Quaternion& rhs)
{
    return !(lhs == rhs);
}

} // namespace Math