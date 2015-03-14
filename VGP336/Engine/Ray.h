#ifndef INCLUDED_ENGINE_RAY_H
#define INCLUDED_ENGINE_RAY_H

//====================================================================================================
// Filename:	Ray.h
// Created by:	Tyler Staples
// Description:	Struct for a Ray.
//====================================================================================================

//====================================================================================================
// Structs
//====================================================================================================

namespace Math
{

struct Ray
{
    Vector3 org;    // origin   
    Vector3 dir;    // direction

    Ray() {}
    Ray(const Vector3& origin, const Vector3& direction)
        : org(origin), dir(direction)
    {}
};

} // namespace Math

#endif // #ifndef INCLUDED_ENGINE_RAY_H