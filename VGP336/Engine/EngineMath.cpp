//====================================================================================================
// Filename:	EngineMath.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "EngineMath.h"

namespace Math
{

//====================================================================================================
// Constants
//====================================================================================================

const f32 kPi			= 3.14159265358979f;
const f32 kTwoPi		= 6.28318530717958f;
const f32 kPiByTwo		= 1.57079632679489f;
const f32 kDegToRad		= kPi / 180.0f;
const f32 kRadToDeg		= 180.0f / kPi;

//====================================================================================================
// Function Definitions
//====================================================================================================

Matrix Matrix::RotationAxis(const Vector3& axis, f32 rad)
{
	const Vector3 u = Normalize(axis);
	const f32 x = u.x;
	const f32 y = u.y;
	const f32 z = u.z;
	const f32 s = sin(rad);
	const f32 c = cos(rad);

	return Matrix
	(
		c + (x * x * (1.0f - c)),
		x * y * (1.0f - c) + z * s,
		x * z * (1.0f - c) - y * s,
		0.0f,

		x * y * (1.0f - c) - z * s,
		c + (y * y * (1.0f - c)),
		y * z * (1.0f - c) + x * s,
		0.0f,

		x * z * (1.0f - c) + y * s,
		y * z * (1.0f - c) - x * s,
		c + (z * z * (1.0f - c)),
		0.0f,

		0.0f,
		0.0f,
		0.0f,
		1.0f
	);
 }

//----------------------------------------------------------------------------------------------------

bool Intersect(const Vector3& point, const AABB& aabb)
{
    if (point.x > aabb.extend.x || point.x < -aabb.extend.x ||
        point.y > aabb.extend.y || point.y < -aabb.extend.y ||
        point.z > aabb.extend.z || point.z < -aabb.extend.z)
    {
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------------------

bool Intersect(const Vector3& p, const OBB& obb)
{
    // Build the transformation matrix to bring the point into the OBB's local space
    Matrix matTrans = Matrix::Translation(obb.center);
    Matrix matRot = Convert(obb.rot);
    Matrix toLocal = Inverse(matRot * matTrans);

    // Transform the point into the box's local space
    Vector3 localPoint = TransformCoord(p, toLocal);

    // We can just treat it as an AABB now since we're in the OBB's local space
    AABB aabb(Vector3::Zero(), obb.extend);
    return Intersect(localPoint, aabb);
}

//----------------------------------------------------------------------------------------------------

bool Intersect(const Ray& ray, const OBB& obb, f32& distance)
{
    // Build transformation matrix to bring the ray into the OBB's local space
    Matrix matTrans = Matrix::Translation(obb.center);
    Matrix matRot = Convert(obb.rot);
    Matrix toLocal = Inverse(matRot * matTrans);

    // Bring the ray into OBB's local space
    Vector3 localOrigin = TransformCoord(ray.org, toLocal);
    Vector3 localDirection = TransformNormal(ray.dir, toLocal);
    Ray localRay(localOrigin, localDirection);

    // Do the intersect test with the ray and local OBB (AABB)
    AABB aabb(Vector3::Zero(), obb.extend);
    f32 dEntry = 0.0f, dExit = 0.0f;
    bool rc = Intersect(localRay, aabb, dEntry, dExit);
    distance = dExit - dEntry;
    return rc;
}

//----------------------------------------------------------------------------------------------------

bool Intersect(const Ray& ray, const AABB& aabb, f32& distEntry, f32& distExit)
{
    Vector3 boxMin = aabb.center - aabb.extend;
    Vector3 boxMax = aabb.center + aabb.extend;
    f32 txmin, txmax, tymin, tymax, tzmin, tzmax;

    f32 divx = 1.0f / ray.dir.x;
    f32 divy = 1.0f / ray.dir.y;
    f32 divz = 1.0f / ray.dir.z;

    if (divx >= 0.0f)
    {
        txmin = (boxMin.x - ray.org.x) * divx;
        txmax = (boxMax.x - ray.org.x) * divx;
    }
    else
    {
        txmin = (boxMax.x - ray.org.x) * divx;
        txmax = (boxMin.x - ray.org.x) * divx;
    }

    if (divy >= 0.0f)
    {
        tymin = (boxMin.y - ray.org.y) * divy;
        tymax = (boxMax.y - ray.org.y) * divy;
    }
    else
    {
        tymin = (boxMax.y - ray.org.y) * divy;
        tymax = (boxMin.y - ray.org.y) * divy;
    }

    if ((txmin > tymax) || (tymin > txmax))
    {
        return false;
    }

    if (tymin > txmin)
    {
        txmin = tymin;
    }
    if (tymax < txmax)
    {
        txmax = tymax;
    }

    if (divz >= 0.0f)
    {
        tzmin = (boxMin.z - ray.org.z) * divz;
        tzmax = (boxMax.z - ray.org.z) * divz;
    }
    else
    {
        tzmin = (boxMax.z - ray.org.z) * divz;
        tzmax = (boxMin.z - ray.org.z) * divz;
    }

    if ((txmin > tzmax) || (tzmin > txmax))
    {
        return false;
    }

    if (tzmin > txmin)
    {
        txmin = tzmin;
    }
    if (tzmax < txmax)
    {
        txmax = tzmax;
    }

    distEntry = txmin;
    distExit = txmax;
    return true;
}

//----------------------------------------------------------------------------------------------------

bool Intersect(const Ray& ray, const Plane& plane, f32& distance)
{
    // Can't assume plane's normal is passed in normalized
    const Vector3 n = Normalize(plane.n);
    const f32 orgDotN = Dot(ray.org, n);
    const f32 dirDotN = Dot(ray.dir, n);

    // Check if the ray and plane are parallel
    if (IsZero(dirDotN - plane.d))
    {
        // Check if the ray is on the plane
        if (IsZero(orgDotN - plane.d))
        {
            distance = 0.0f;
            return true;
        }
        return false;
    }
    distance = (plane.d - orgDotN) / dirDotN;
    return true;
}

// first pass check to see if point is intersecting
// second pass - ray box intersection test (ray in particle's direction)
//  check against a 'slab'
//      min/max x/y form a slab
//      all 4 would be cross by the ray
//      if you hit all the mins first, then the max's, you're intersecting

// get the point on the box where the intersection occurs
// get the normal of that face, then reflect the point over that normal
// hack: set old position to inverse of reflected vector to change direction point travels

bool GetContactPoint(const Ray& ray, const OBB& obb, Vector3& point, Vector3& normal)
{
    // Build transformation matrix to bring the ray into the OBB's local space
    Matrix matTrans = Matrix::Translation(obb.center);
    Matrix matRot = Convert(obb.rot);
    Matrix toWorld = matRot * matTrans;
    Matrix toLocal = Inverse(toWorld);

    Vector3 localOrg = TransformCoord(ray.org, toLocal);
    Vector3 localDir = TransformNormal(ray.dir, toLocal);
    Ray localRay(localOrg, localDir);

    // Build the planes representing the OBB in local space
    Plane planes[] = 
    {
        Plane( 0.0f,  0.0f, -1.0f, obb.extend.z),
        Plane( 0.0f,  0.0f,  1.0f, obb.extend.z),
        Plane( 0.0f, -1.0f,  1.0f, obb.extend.y),
        Plane( 0.0f,  1.0f,  1.0f, obb.extend.y),
        Plane(-1.0f,  0.0f,  1.0f, obb.extend.x),
        Plane( 1.0f,  0.0f,  1.0f, obb.extend.x)
    };

    const u32 numPlanes = 6;
    u32 numIntersections = 0;
    for (u32 i=0; i < numPlanes; ++i)
    {
        const Plane& plane = planes[i];

        // Check the plane is the nearest ones
        const f32 d = Dot(localOrg, plane.n);
        if (d > plane.d)
        {
            f32 distance = 0.0f;
            if (Intersect(localRay, plane, distance) && distance >= 0.0f)
            {
                Vector3 pt = localOrg + (localDir * distance);

                // add small buffer to make sure the point is close enough to the box
                if (fabs(pt.x) <= obb.extend.x + 0.01f &&
                    fabs(pt.y) <= obb.extend.y + 0.01f &&
                    fabs(pt.z) <= obb.extend.z + 0.01f)
                {
                    point = pt;
                    normal = plane.n;
                    ++numIntersections;
                }
            }
        }
    }

    if (numIntersections == 0)
    {
        return false;
    }

    // Bring the point and normal back into world space
    point = TransformCoord(point, toWorld);
    normal = TransformNormal(point, toWorld);
    return true;
}

} // namespace Math