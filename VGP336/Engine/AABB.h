#ifndef INCLUDED_ENGINE_AABB_H
#define INCLUDED_ENGINE_AABB_H

//====================================================================================================
// Filename:	AABB.h
// Created by:	Tyler Staples
// Description:	Struct for an axis aligned bounding box.
//====================================================================================================

//====================================================================================================
// Structs
//====================================================================================================

namespace Math
{

struct AABB
{
    Vector3 center;
    Vector3 extend;

    AABB() 
        : center(0.0f, 0.0f, 0.0f), extend(1.0f, 1.0f, 1.0f) 
    {}
    AABB(f32 cx, f32 cy, f32 cz, const Vector3& _extend)
        : center(cx, cy, cz), extend(_extend)
    {}
    AABB(const Vector3& _center, const Vector3& _extend)
        : center(_center), extend(_extend)
    {}

    inline bool Contains(const Vector3& v)
    {
        return (v.x < extend.x &&
                v.y < extend.y &&
                v.z < extend.z);
    }

    friend std::ostream& operator<<(std::ostream& os, const AABB& v)
    {
        // TODO
        return os;
    }
};

} // namespace Math

#endif // #ifndef INCLUDED_ENGINE_AABB_H