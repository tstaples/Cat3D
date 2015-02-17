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
    AABB(const Vector3& _center, const Vector3& _extend)
        : center(_center), extend(_extend)
    {}
};

} // namespace Math

#endif // #ifndef INCLUDED_ENGINE_AABB_H