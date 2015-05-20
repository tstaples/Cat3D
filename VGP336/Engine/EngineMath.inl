//====================================================================================================
// Filename:	EngineMath.inl
// Created by:	Peter Chan
//====================================================================================================

namespace Math
{

//====================================================================================================
// Function Definitions
//====================================================================================================

template <typename T>
inline T Min(T a, T b)
{
	return (a > b) ? b : a;
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline T Max(T a, T b)
{
	return (a < b) ? b : a;
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline T Clamp(T value, T min, T max)
{
	return Max(min, Min(max, value));
}

//----------------------------------------------------------------------------------------------------

inline f32 Abs(f32 value)
{
	return (value >= 0.0f) ? value : -value;
}

//----------------------------------------------------------------------------------------------------

inline f32 Sign(f32 value)
{
	return (value >= 0.0f) ? 1.0f : -1.0f;
}

//----------------------------------------------------------------------------------------------------

inline f32 Sqr(f32 value)
{
	return value * value;
}

//----------------------------------------------------------------------------------------------------

inline f32 Sqrt(f32 value)
{
	ASSERT(value >= 0.0f, "[Math] Cannot square root a negative number!");
	return sqrt(value);
}

//----------------------------------------------------------------------------------------------------

inline f32 InvSqrt(f32 value)
{
    return (1.0f / (value * value));
}

//----------------------------------------------------------------------------------------------------

inline bool Compare(f32 a, f32 b, f32 epsilon)
{
	return Abs(a - b) < epsilon;
}

//----------------------------------------------------------------------------------------------------

inline bool IsZero(f32 value)
{
	return Compare(value, 0.0f);
}

//----------------------------------------------------------------------------------------------------

inline bool IsZero(const Vector2& v)
{
	return IsZero(v.x) && IsZero(v.y);
}

//----------------------------------------------------------------------------------------------------

inline bool IsZero(const Vector3& v)
{
	return IsZero(v.x) && IsZero(v.y) && IsZero(v.z);
}

//----------------------------------------------------------------------------------------------------

// http://www.musicdsp.org/showone.php?id=249
inline bool IsPositive(f32 f)
{
	if (IsZero(f))
		return false;
	
	f32 r = 1.0f;
    (s32&)r |= ((s32&)f & 0x80000000); // mask sign bit in f, set it in r if necessary
	return (r > 0.0f);
}

//----------------------------------------------------------------------------------------------------

inline f32 MagnitudeSqr(const Vector3& v)
{
	return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
}

//----------------------------------------------------------------------------------------------------

inline f32 MagnitudeSqr(const Vector2& v)
{
	return (v.x * v.x) + (v.y * v.y);
}

//----------------------------------------------------------------------------------------------------

inline f32 Magnitude(const Vector3& v)
{
	return Sqrt(MagnitudeSqr(v));
}

//----------------------------------------------------------------------------------------------------

inline f32 Magnitude(const Vector2& v)
{
	return Sqrt(MagnitudeSqr(v));
}

//----------------------------------------------------------------------------------------------------

inline Vector3 Normalize(const Vector3& v)
{
	ASSERT(!IsZero(v), "[Math] Cannot normalize zero length vector!");
	const f32 inv = 1.0f / Magnitude(v);
	return v * inv;
}

//----------------------------------------------------------------------------------------------------

inline Vector2 Normalize(const Vector2& v)
{
    ASSERT(!IsZero(v), "[Math] Cannot normalize zero length vector!");
	const f32 inv = 1.0f / Magnitude(v);
	return v * inv;
}

//----------------------------------------------------------------------------------------------------

inline Quaternion Normalize(const Quaternion& q)
{
    const f32 n = Dot(q, q);
    if (Compare(n, 1.0f))
    {
        // q is already normalized
        return q;
    }
    return (q * InvSqrt(n));
}

//----------------------------------------------------------------------------------------------------

inline f32 DistanceSqr(const Vector3& a, const Vector3& b)
{
	return MagnitudeSqr(a - b);
}

//----------------------------------------------------------------------------------------------------

inline f32 Distance(const Vector3& a, const Vector3& b)
{
	return Sqrt(DistanceSqr(a, b));
}

//----------------------------------------------------------------------------------------------------

inline f32 Dot(const Quaternion& q1, const Quaternion& q2)
{
    return (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);
}

//----------------------------------------------------------------------------------------------------

inline f32 Dot(const Vector3& a, const Vector3& b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

//----------------------------------------------------------------------------------------------------

inline Vector3 Cross(const Vector3& a, const Vector3& b)
{
	return Vector3
	(
		(a.y * b.z) - (a.z * b.y),
		(a.z * b.x) - (a.x * b.z),
		(a.x * b.y) - (a.y * b.x)
	);
}

//----------------------------------------------------------------------------------------------------

inline Vector3 Project(const Vector3& v, const Vector3& n)
{
	return n * (Dot(v, n) / Dot(n, n));
}

//----------------------------------------------------------------------------------------------------

inline Vector3 Project(const Vector3& v, const Plane& p)
{
    f32 dist = Dot(v, p.n);
    return (v - (p.n * dist));
}

//----------------------------------------------------------------------------------------------------

inline f32 Determinant(const Matrix& m)
{
	f32 det = 0.0f;
	det  = (m._11 * (m._22 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._32 * m._44 - (m._42 * m._34)) + m._24 * (m._32 * m._43 - (m._42 * m._33))));
	det -= (m._12 * (m._21 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._43 - (m._41 * m._33))));
	det += (m._13 * (m._21 * (m._32 * m._44 - (m._42 * m._34)) - m._22 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._42 - (m._41 * m._32))));
	det -= (m._14 * (m._21 * (m._32 * m._43 - (m._42 * m._33)) - m._22 * (m._31 * m._43 - (m._41 * m._33)) + m._23 * (m._31 * m._42 - (m._41 * m._32))));
	return det;
}

//----------------------------------------------------------------------------------------------------

inline Matrix Adjoint(const Matrix& m)
{
	return Matrix
	(
		 (m._22 * ((m._33 * m._44) - (m._43 * m._34)) - m._23 * ((m._32 * m._44) - (m._42 * m._34)) + m._24 * ((m._32 * m._43) - (m._42 * m._33))),
		-(m._12 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._32 * m._44) - (m._42 * m._34)) + m._14 * ((m._32 * m._43) - (m._42 * m._33))),
		 (m._12 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._22 * m._44) - (m._42 * m._24)) + m._14 * ((m._22 * m._43) - (m._42 * m._23))),
		-(m._12 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._22 * m._34) - (m._32 * m._24)) + m._14 * ((m._22 * m._33) - (m._32 * m._23))),

		-(m._21 * ((m._33 * m._44) - (m._43 * m._34)) - m._31 * ((m._23 * m._44) - (m._24 * m._43)) + m._41 * ((m._23 * m._34) - (m._24 * m._33))),
		 (m._11 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._31 * m._44) - (m._41 * m._34)) + m._14 * ((m._31 * m._43) - (m._41 * m._33))),
		-(m._11 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._43) - (m._41 * m._23))),
		 (m._11 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._21 * m._34) - (m._31 * m._24)) + m._14 * ((m._21 * m._33) - (m._31 * m._23))),

		 (m._21 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._22 * m._44) - (m._42 * m._24)) + m._41 * ((m._22 * m._34) - (m._32 * m._24))),
		-(m._11 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._12 * m._44) - (m._42 * m._14)) + m._41 * ((m._12 * m._34) - (m._32 * m._14))),
		 (m._11 * ((m._22 * m._44) - (m._42 * m._24)) - m._12 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._42) - (m._41 * m._22))),
		-(m._11 * ((m._22 * m._34) - (m._32 * m._24)) - m._21 * ((m._12 * m._34) - (m._32 * m._14)) + m._31 * ((m._12 * m._24) - (m._22 * m._14))),

		-(m._21 * ((m._32 * m._43) - (m._42 * m._33)) - m._31 * ((m._22 * m._43) - (m._42 * m._23)) + m._41 * ((m._22 * m._33) - (m._32 * m._23))),
		 (m._11 * ((m._32 * m._43) - (m._42 * m._33)) - m._12 * ((m._31 * m._43) - (m._41 * m._33)) + m._13 * ((m._31 * m._42) - (m._41 * m._32))),
		-(m._11 * ((m._22 * m._43) - (m._42 * m._23)) - m._12 * ((m._21 * m._43) - (m._41 * m._23)) + m._13 * ((m._21 * m._42) - (m._41 * m._22))),
		 (m._11 * ((m._22 * m._33) - (m._32 * m._23)) - m._12 * ((m._21 * m._33) - (m._31 * m._23)) + m._13 * ((m._21 * m._32) - (m._31 * m._22)))
	);
}

//----------------------------------------------------------------------------------------------------

inline Matrix Inverse(const Matrix& m)
{
	const f32 determinant = Determinant(m);
	ASSERT(!IsZero(determinant), "[Math] Cannot find the inverse of matrix. Determinant equals 0.0!");
	const f32 invDet = 1.0f / determinant;
	return Adjoint(m) * invDet;
}

//----------------------------------------------------------------------------------------------------

inline Matrix Transpose(const Matrix& m)
{
	return Matrix
	(
		m._11, m._21, m._31, m._41,
		m._12, m._22, m._32, m._42,
		m._13, m._23, m._33, m._43,
		m._14, m._24, m._34, m._44
	);
}

//----------------------------------------------------------------------------------------------------

inline Vector3 TransformCoord(const Vector3& v, const Matrix& m)
{
    f32 x = v.x * m._11 + v.y * m._21 + v.z * m._31 + m._41;
    f32 y = v.x * m._12 + v.y * m._22 + v.z * m._32 + m._42;
    f32 z = v.x * m._13 + v.y * m._23 + v.z * m._33 + m._43;
    f32 w = v.x * m._14 + v.y * m._24 + v.z * m._34 + m._44;

    return Vector3( x/w, y/w, z/w );

	/*return Vector3
	(
		v.x * m._11 + v.y * m._21 + v.z * m._31 + m._41,
		v.x * m._12 + v.y * m._22 + v.z * m._32 + m._42,
		v.x * m._13 + v.y * m._23 + v.z * m._33 + m._43
	);*/
}

//----------------------------------------------------------------------------------------------------

inline Vector3 TransformNormal(const Vector3& v, const Matrix& m)
{
	return Vector3
	(
		v.x * m._11 + v.y * m._21 + v.z * m._31,
		v.x * m._12 + v.y * m._22 + v.z * m._32,
		v.x * m._13 + v.y * m._23 + v.z * m._33
	);
}

//----------------------------------------------------------------------------------------------------

inline Vector3 Lerp(const Vector3& v0, const Vector3& v1, f32 t)
{
	return v0 + ((v1 - v0) * t);
}

//----------------------------------------------------------------------------------------------------

inline Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, f32 t)
{
    Quaternion q;
    q.x = q1.x + ((q2.x - q1.x) * t);
    q.y = q1.y + ((q2.y - q1.y) * t);
    q.z = q1.z + ((q2.z - q1.z) * t);
    q.w = q1.w + ((q2.w - q1.w) * t);
    return q;
}

//----------------------------------------------------------------------------------------------------

inline Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, f32 t)
{
    Quaternion qa = q1;
    Quaternion qb = q2;
    f32 angle = Dot(q1, q2);

    // Make sure we use the short rotation
    if (angle < 0.0f)
    {
        qa = qa * -1.0f;
        angle *= -1.0f;
    }
    else if (angle > 0.999f)
    {
        // Just use regular lerp
        return Normalize(Lerp(q1, q2, t));
    }

    const f32 theta         = acosf(angle);
    const f32 invSinTheta   = 1.0f / sinf(theta);
    const f32 scale         = sinf(theta * (1.0f - t)) * invSinTheta;
    const f32 invScale      = sinf(theta * t) * invSinTheta;
    return Quaternion((qa * scale) + (qb * invScale));
}

//----------------------------------------------------------------------------------------------------

inline Matrix Convert(const Quaternion& q)
{
    const f32 x = q.x;
    const f32 y = q.y;
    const f32 z = q.z;
    const f32 w = q.w;

    Matrix m;
    m._11 = 1.0f - (2.0f * Sqr(y)) - (2.0f * Sqr(z));
    m._12 = (2.0f * x * y) + (2.0f * w * z);
    m._13 = (2.0f * x * z) - (2.0f * w * y);

    m._21 = (2.0f * x * y) - (2.0f * w * z);
    m._22 = 1.0f - (2.0f * Sqr(x)) - (2.0f * Sqr(z));
    m._23 = (2.0f * y * z) + (2.0f * w * x);

    m._31 = (2.0f * x * z) + (2.0f * w * y);
    m._32 = (2.0f * y * z) - (2.0f * w * x);
    m._33 = 1.0f - (2.0f * Sqr(x)) - (2.0f * Sqr(y));
    return m;
}

//----------------------------------------------------------------------------------------------------

inline Quaternion Inverse(const Quaternion& q)
{
    return Quaternion(-q.x, -q.y, -q.z, -q.w);
}

//----------------------------------------------------------------------------------------------------

inline Quaternion Conjugate(const Quaternion& q)
{
    // Invert vector part
    return Quaternion(-q.x, -q.y, -q.z, q.w);
}

//----------------------------------------------------------------------------------------------------

// http://gamedev.stackexchange.com/a/50545/51098
inline Vector3 RotateVector(const Vector3& v, const Quaternion& q)
{
    Vector3 qv = q.GetVectorPart();

    Vector3 rot = qv * 2.0f * Dot(qv, v)
                + v * (Sqr(q.w) - Dot(qv, qv)) 
                + Cross(v, qv) * (q.w * 2.0f);
    return rot;
}

//----------------------------------------------------------------------------------------------------

// http://gamedev.stackexchange.com/a/50968/51098
inline Vector3 GetRotation(const Matrix& m)
{
    Vector3 rot;
    if (m._11 == 1.0f)
    {
        rot.x = atan2f(m._13, m._34);
        rot.y = 0.0f;
        rot.z = 0.0f;
    }
    else if (m._11 == -1.0f)
    {
        rot.x = atan2f(-m._31, m._11);
        rot.y = 0.0f;
        rot.z = 0.0f;
    }
    else
    {
        rot.x = atan2f(-m._31, m._11) * kRadToDeg;
        rot.y = asinf(m._21) * kRadToDeg;
        rot.z = atan2f(-m._23, m._22) * kRadToDeg;
    }
    return rot;
}

} // namespace Math
